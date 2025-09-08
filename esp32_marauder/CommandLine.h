#pragma once

#ifndef CommandLine_h
#define CommandLine_h

#include "configs.h"

#ifdef HAS_SCREEN
  #include "MenuFunctions.h"
  #include "Display.h"
#endif 

#include "WiFiScan.h"
#ifdef HAS_SD
  #include "SDInterface.h"
#endif
#include "settings.h"
#include "LedInterface.h"

#ifdef HAS_SCREEN
  extern MenuFunctions menu_function_obj;
  extern Display display_obj;
#endif

extern WiFiScan wifi_scan_obj;
#ifdef HAS_SD
  extern SDInterface sd_obj;
#endif
extern Settings settings_obj;
extern LedInterface led_obj;
extern std::vector<AccessPoint> access_points;
extern std::vector<AirTag> airtags;
extern std::vector<ssid> ssids;
extern std::vector<Station> stations;
extern std::vector<IPAddress> ipList;
extern std::vector<ProbeReqSsid> probe_req_ssids;
extern const String PROGMEM version_number;
extern const String PROGMEM board_target;

//// Command keywords (unchanged) ...
// [snip for brevity]

class CommandLine {
  private:
    String getSerialInput();
    std::vector<String> parseCommand(String input, char* delim);
    String toLowerCase(String str);
    void filterAccessPoints(String filter);
    bool checkValueExists(std::vector<String>* cmd_args_list, int index);
    bool inRange(int max, int index);
    bool apSelected();
    bool hasSSIDs();
    void showCounts(int selected, int unselected = -1);
    int argSearch(std::vector<String>* cmd_args, String key);

    // New command‐handling methods
    void runCommand(String input);
    void showHelp();
    void handleScanCommand(std::vector<String>* cmd_args);
    void handleAttackCommand(std::vector<String>* cmd_args);
    void handleSelectCommand(std::vector<String>* cmd_args);
    void handleClearCommand(std::vector<String>* cmd_args);
    void handleListCommand(std::vector<String>* cmd_args);

    const char* ascii_art =
    "\r\n"
    "              @@@@@@                        \r\n"
    "              @@@@@@@@                      \r\n"
    "              @@@@@@@@@@@                   \r\n"
    "             @@@@@@  @@@@@@                 \r\n"
    "          @@@@@@@      @@@@@@@              \r\n"
    "        @@@@@@            @@@@@@            \r\n"
    "     @@@@@@@                @@@@@@@         \r\n"
    "   @@@@@@                      @@@@@@       \r\n"
    "@@@@@@@              @@@@@@@@@@@@@@@@       \r\n"
    "@@@@@                 @@@@@@@@@@@@@@@       \r\n"
    "@@@@@                   @@@@@@@             \r\n"
    "@@@@@                      @@@@@@           \r\n"
    "@@@@@@                       @@@@@@@        \r\n"
    "  @@@@@@                        @@@@@@@@@@@@\r\n"
    "    @@@@@@@                          @@@@@@ \r\n"
    "       @@@@@@                     @@@@@@    \r\n"
    "         @@@@@@@                @@@@@@      \r\n"
    "            @@@@@@           @@@@@@         \r\n"
    "              @@@@@@@      @@@@@@           \r\n"
    "                 @@@@@@ @@@@@@              \r\n"
    "                   @@@@@@@@@                \r\n"
    "                      @@@@@@                \r\n"
    "                        @@@@                \r\n"
    "\r\n";

  public:
    CommandLine();

    void RunSetup();
    void main(uint32_t currentTime);
};

#endif
