#include "CommandLine.h"

CommandLine::CommandLine() {
}

void CommandLine::RunSetup() {
  Serial.println(this->ascii_art);

  Serial.println(F("\n\n--------------------------------\n"));
  Serial.println(F("         ESP32 Marauder      \n"));
  Serial.println("            " + version_number + "\n");
  Serial.println(F("       By: justcallmekoko\n"));
  Serial.println(F("--------------------------------\n\n"));
  
  Serial.print("> ");
}

String CommandLine::getSerialInput() {
  String input = "";

  if (Serial.available() > 0)
    input = Serial.readStringUntil('\n');

  input.trim();
  return input;
}

void CommandLine::main(uint32_t currentTime) {
  String input = this->getSerialInput();

  this->runCommand(input);

  if (input != "")
    Serial.print("> ");
}

std::vector<String> CommandLine::parseCommand(String input, char* delim) {
  std::vector<String> cmd_args;

  bool inQuote = false;
  bool inApostrophe = false;
  String buffer = "";

  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);

    if (c == '"') {
      if (inApostrophe) {
        buffer += c;
      } else {
        inQuote = !inQuote;
      }
    } else if (c == '\'') {
      if (inQuote) {
        buffer += c;
      } else {
        inApostrophe = !inApostrophe;
      }
    } else if (!inQuote && !inApostrophe && strchr(delim, c) != 0) {
      cmd_args.push_back(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  if (!buffer.isEmpty()) {
    cmd_args.push_back(buffer);
  }

  return cmd_args;
}

int CommandLine::argSearch(std::vector<String>* cmd_args_list, String key) {
  for (int i = 0; i < cmd_args_list->size(); i++) {
    if ((*cmd_args_list)[i] == key)
      return i;
  }
  return -1;
}

bool CommandLine::checkValueExists(std::vector<String>* cmd_args_list, int index) {
  if (index < cmd_args_list->size() - 1)
    return true;
  return false;
}

bool CommandLine::inRange(int max, int index) {
  if ((index >= 0) && (index < max))
    return true;

  return false;
}

bool CommandLine::apSelected() {
  for (int i = 0; i < access_points.size(); i++) {
    if (access_points[i].selected)
      return true;
  }
  return false;
}

bool CommandLine::hasSSIDs() {
  if (ssids.size() == 0)
    return false;

  return true;
}

void CommandLine::showCounts(int selected, int unselected) {
  Serial.print((String)selected + " selected");
  
  if (unselected != -1) 
    Serial.print(", " + (String)unselected + " unselected");
  
  Serial.println("");
}

String CommandLine::toLowerCase(String str) {
  String result = str;
  for (int i = 0; i < str.length(); i++) {
    int charValue = str.charAt(i);
    if (charValue >= 65 && charValue <= 90) {
      charValue += 32;
      result.setCharAt(i, char(charValue));
    }
  }
  return result;
}

void CommandLine::filterAccessPoints(String filter) {
  int count_selected = 0;
  int count_unselected = 0;

  std::vector<String> filters;
  int start = 0;
  int end = filter.indexOf(" or ");
  while (end != -1) {
    filters.push_back(filter.substring(start, end));
    start = end + 4;
    end = filter.indexOf(" or ", start);
  }
  filters.push_back(filter.substring(start));

  for (int i = 0; i < access_points.size(); i++) {
    bool matchesFilter = false;
    for (int j = 0; j < filters.size(); j++) {
      String f = toLowerCase(filters[j]);
      if (f.substring(0, 7) == "equals ") {
        String ssidEquals = f.substring(7);
        if ((ssidEquals.charAt(0) == '"' && ssidEquals.charAt(ssidEquals.length() - 1) == '"' && ssidEquals.length() > 1) ||
            (ssidEquals.charAt(0) == '\'' && ssidEquals.charAt(ssidEquals.length() - 1) == '\'' && ssidEquals.length() > 1)) {
          ssidEquals = ssidEquals.substring(1, ssidEquals.length() - 1);
        }
        if (access_points[i].essid.equalsIgnoreCase(ssidEquals)) {
          matchesFilter = true;
          break;
        }
      } else if (f.substring(0, 9) == "contains ") {
        String ssidContains = f.substring(9);
        if ((ssidContains.charAt(0) == '"' && ssidContains.charAt(ssidContains.length() - 1) == '"' && ssidContains.length() > 1) ||
            (ssidContains.charAt(0) == '\'' && ssidContains.charAt(ssidContains.length() - 1) == '\'' && ssidContains.length() > 1)) {
          ssidContains = ssidContains.substring(1, ssidContains.length() - 1);
        }
        String essid = toLowerCase(access_points[i].essid);
        if (essid.indexOf(ssidContains) != -1) {
          matchesFilter = true;
          break;
        }
      }
    }
    AccessPoint new_ap = access_points[i];
    new_ap.selected = matchesFilter;
    access_points[i] = new_ap;

    if (matchesFilter) {
      count_selected++;
    } else {
      count_unselected++;
    }
  }

  this->showCounts(count_selected, count_unselected);
}
// Add this method to CommandLine.cpp, preferably after the filterAccessPoints method

void CommandLine::runCommand(String input) {
  if (input.length() == 0) {
    return;
  }

  // Parse the command into arguments
  char delim[] = " ";
  std::vector<String> cmd_args = this->parseCommand(input, delim);
  
  if (cmd_args.size() == 0) {
    return;
  }

  String command = this->toLowerCase(cmd_args[0]);

  // Handle different commands
  if (command == "help") {
    this->showHelp();
  }
  else if (command == "scan") {
    this->handleScanCommand(&cmd_args);
  }
  else if (command == "attack") {
    this->handleAttackCommand(&cmd_args);
  }
  else if (command == "select") {
    this->handleSelectCommand(&cmd_args);
  }
  else if (command == "clear") {
    this->handleClearCommand(&cmd_args);
  }
  else if (command == "list") {
    this->handleListCommand(&cmd_args);
  }
  else if (command == "stop") {
    wifi_scan_obj.StartScan(WIFI_SCAN_OFF);
    Serial.println("Stopped current scan/attack");
  }
  else if (command == "info") {
    wifi_scan_obj.RunInfo();
  }
  else if (command == "reboot") {
    ESP.restart();
  }
  else {
    Serial.println("Unknown command: " + command);
    Serial.println("Type 'help' for available commands");
  }
}

// You'll also need to add these helper methods:

void CommandLine::showHelp() {
  Serial.println("Available commands:");
  Serial.println("  help                 - Show this help message");
  Serial.println("  scan ap              - Scan for access points");
  Serial.println("  scan station         - Scan for stations");
  Serial.println("  attack deauth        - Deauth attack");
  Serial.println("  attack beacon        - Beacon spam attack");
  Serial.println("  select ap <filter>   - Select access points");
  Serial.println("  clear ap             - Clear access points");
  Serial.println("  list ap              - List access points");
  Serial.println("  stop                 - Stop current operation");
  Serial.println("  info                 - Show device info");
  Serial.println("  reboot               - Restart device");
}

void CommandLine::handleScanCommand(std::vector<String>* cmd_args) {
  if (cmd_args->size() < 2) {
    Serial.println("Usage: scan <type>");
    Serial.println("Types: ap, station, probe, deauth");
    return;
  }

  String scan_type = this->toLowerCase((*cmd_args)[1]);
  
  if (scan_type == "ap") {
    wifi_scan_obj.StartScan(WIFI_SCAN_AP);
    Serial.println("Started AP scan");
  }
  else if (scan_type == "station") {
    wifi_scan_obj.StartScan(WIFI_SCAN_STATION);
    Serial.println("Started station scan");
  }
  else if (scan_type == "probe") {
    wifi_scan_obj.StartScan(WIFI_SCAN_PROBE);
    Serial.println("Started probe scan");
  }
  else if (scan_type == "deauth") {
    wifi_scan_obj.StartScan(WIFI_SCAN_DEAUTH);
    Serial.println("Started deauth scan");
  }
  else {
    Serial.println("Unknown scan type: " + scan_type);
  }
}

void CommandLine::handleAttackCommand(std::vector<String>* cmd_args) {
  if (cmd_args->size() < 2) {
    Serial.println("Usage: attack <type>");
    Serial.println("Types: deauth, beacon, probe");
    return;
  }

  String attack_type = this->toLowerCase((*cmd_args)[1]);
  
  if (attack_type == "deauth") {
    if (!this->apSelected()) {
      Serial.println("No access points selected. Use 'select ap' first.");
      return;
    }
    wifi_scan_obj.StartScan(WIFI_ATTACK_DEAUTH);
    Serial.println("Started deauth attack");
  }
  else if (attack_type == "beacon") {
    wifi_scan_obj.StartScan(WIFI_ATTACK_BEACON_SPAM);
    Serial.println("Started beacon spam attack");
  }
  else if (attack_type == "probe") {
    wifi_scan_obj.StartScan(WIFI_ATTACK_AUTH);
    Serial.println("Started probe attack");
  }
  else {
    Serial.println("Unknown attack type: " + attack_type);
  }
}

void CommandLine::handleSelectCommand(std::vector<String>* cmd_args) {
  if (cmd_args->size() < 2) {
    Serial.println("Usage: select <type> [filter]");
    Serial.println("Types: ap, station");
    return;
  }

  String select_type = this->toLowerCase((*cmd_args)[1]);
  
  if (select_type == "ap") {
    if (cmd_args->size() >= 3) {
      String filter = "";
      for (int i = 2; i < cmd_args->size(); i++) {
        if (i > 2) filter += " ";
        filter += (*cmd_args)[i];
      }
      this->filterAccessPoints(filter);
    } else {
      Serial.println("Usage: select ap <filter>");
      Serial.println("Example: select ap contains \"wifi\"");
    }
  }
  else {
    Serial.println("Unknown select type: " + select_type);
  }
}

void CommandLine::handleClearCommand(std::vector<String>* cmd_args) {
  if (cmd_args->size() < 2) {
    Serial.println("Usage: clear <type>");
    Serial.println("Types: ap, station, ssid");
    return;
  }

  String clear_type = this->toLowerCase((*cmd_args)[1]);
  
  if (clear_type == "ap") {
    wifi_scan_obj.RunClearAPs();
    Serial.println("Cleared access points");
  }
  else if (clear_type == "station") {
    wifi_scan_obj.RunClearStations();
    Serial.println("Cleared stations");
  }
  else if (clear_type == "ssid") {
    wifi_scan_obj.RunClearSSIDs();
    Serial.println("Cleared SSIDs");
  }
  else {
    Serial.println("Unknown clear type: " + clear_type);
  }
}

void CommandLine::handleListCommand(std::vector<String>* cmd_args) {
  if (cmd_args->size() < 2) {
    Serial.println("Usage: list <type>");
    Serial.println("Types: ap, station");
    return;
  }

  String list_type = this->toLowerCase((*cmd_args)[1]);
  
  if (list_type == "ap") {
    Serial.println("\nAccess Points:");
    Serial.println("ID | ESSID | BSSID | Channel | RSSI | Selected");
    Serial.println("---|-------|-------|---------|------|----------");
    
    for (int i = 0; i < access_points.size(); i++) {
      Serial.printf("%2d | %-20s | %02X:%02X:%02X:%02X:%02X:%02X | %2d | %4d | %s\n",
        i,
        access_points[i].essid.c_str(),
        access_points[i].bssid[0], access_points[i].bssid[1], access_points[i].bssid[2],
        access_points[i].bssid[3], access_points[i].bssid[4], access_points[i].bssid[5],
        access_points[i].channel,
        access_points[i].rssi,
        access_points[i].selected ? "Yes" : "No"
      );
    }
    Serial.println("Total: " + String(access_points.size()) + " access points");
  }
  else {
    Serial.println("Unknown list type: " + list_type);
  }
}

// All other CommandLine methods remain the same.
