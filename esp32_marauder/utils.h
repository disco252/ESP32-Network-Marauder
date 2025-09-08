#pragma once
#ifndef utils_h
#define utils_h

#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include "configs.h"
#include "esp_heap_caps.h"

// --- Data and Structs ---
struct mac_addr {
   unsigned char bytes[6];
};

struct Station {
  uint8_t mac[6];
  bool selected;
  uint16_t packets;
  uint16_t ap;
};

struct ProbeReqSsid {
    String essid;
    bool selected;
    uint8_t requests;
};

// --- OUI arrays --- (keep them in header as they are 'const')
extern const char apple_ouis[][9] PROGMEM;
extern const char asus_ouis[][9] PROGMEM;
extern const char belkin_ouis[][9] PROGMEM;
extern const char cisco_ouis[][9] PROGMEM;
extern const char dlink_ouis[][9] PROGMEM;
extern const char google_ouis[][9] PROGMEM;
extern const char huawei_ouis[][9] PROGMEM;
extern const char lg_ouis[][9] PROGMEM;
extern const char linksys_ouis[][9] PROGMEM;
extern const char netgear_ouis[][9] PROGMEM;
extern const char oneplus_ouis[][9] PROGMEM;
extern const char samsung_ouis[][9] PROGMEM;
extern const char sony_ouis[][9] PROGMEM;
extern const char tplink_ouis[][9] PROGMEM;
extern const char xiaomi_ouis[][9] PROGMEM;

// --- Function Prototypes ---
// Always declare, never define in header unless 'inline'
uint8_t getDRAMUsagePercent();

#ifdef HAS_PSRAM
uint8_t getPSRAMUsagePercent();
#endif

String byteArrayToHexString(const std::vector<uint8_t>& byteArray);
std::vector<uint8_t> hexStringToByteArray(const String& hexString);

void generateRandomName(char *name, size_t length);
const char* generateRandomName();
void generateRandomMac(uint8_t* mac);

String macToString(const Station& station);
String macToString(uint8_t macAddr[6]);
void convertMacStringToUint8(const String& macStr, uint8_t macAddr[6]);

const char* getManufacturer(const char *addr);
String replaceOUIWithManufacturer(const char *sta_addr);

IPAddress getNextIP(IPAddress currentIP, IPAddress subnetMask);
IPAddress getPrevIP(IPAddress currentIP, IPAddress subnetMask, uint16_t stepsBack);
uint16_t getNextPort(uint16_t port);

#endif
