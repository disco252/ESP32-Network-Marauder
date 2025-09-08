#pragma once

#ifndef lang_var_h
#define lang_var_h

#include "configs.h"

// Starting window texts
PROGMEM const char text0_0[] = "Giving room for HardwareSerial...";
PROGMEM const char text0_1[] = "Started Serial";
PROGMEM const char text0_2[] = "Checked RAM";
PROGMEM const char text0_3[] = "Initialized SD Card";
PROGMEM const char text0_4[] = "Failed to Initialize SD Card";
PROGMEM const char text0_5[] = "Checked battery configuration";
PROGMEM const char text0_6[] = "Initialized temperature interface";
PROGMEM const char text0_7[] = "Initialized LED Interface";
PROGMEM const char text0_8[] = "Starting...";

// Single library (action) texts/Often used
PROGMEM const char text00[] = "Battery Level changed: ";
PROGMEM const char text01[] = "file closed";
PROGMEM const char text02[] = "Failed to open file '";
PROGMEM const char text03[] = "ON";
PROGMEM const char text04[] = "OFF";
PROGMEM const char text05[] = "Load";
PROGMEM const char text06[] = "Save As";
PROGMEM const char text07[] = "Exit";
PROGMEM const char text08[] = "Settings";
PROGMEM const char text09[] = "Back";
PROGMEM const char text10[] = "Channel:";
PROGMEM const char text11[] = "Touch screen to exit";
PROGMEM const char text12[] = "Cancel";
PROGMEM const char text13[] = "Save";
PROGMEM const char text14[] = "Yes";
PROGMEM const char text15[] = "Opening /update.bin...";
PROGMEM const char text16[] = "Close";
PROGMEM const char text17[] = "FAIL";
PROGMEM const char text18[] = "packets/sec: ";

// Menufunctions.cpp texts
PROGMEM const char text1_0[] = "SSID List";
PROGMEM const char text1_1[] = "Add SSIDs";
PROGMEM const char text1_2[] = "SSID: ";
PROGMEM const char text1_3[] = "Password:";
PROGMEM const char text1_4[] = "Setting disabled";
PROGMEM const char text1_5[] = "Setting on";
PROGMEM const char text1_6[] = "ESP32 Marauder ";
PROGMEM const char text1_7[] = "WiFi ";
PROGMEM const char text1_8[] = "Bad USB ";
PROGMEM const char text1_9[] = "Device ";
PROGMEM const char text1_10[] = "General Apps ";
PROGMEM const char text1_11[] = "Updating... ";
PROGMEM const char text1_12[] = "Select Method ";
PROGMEM const char text1_13[] = "Confirm Update ";
PROGMEM const char text1_14[] = "ESP8266 Update ";
PROGMEM const char text1_15[] = "Update Firmware ";
PROGMEM const char text1_16[] = "Language ";
PROGMEM const char text1_17[] = "Device Info ";
PROGMEM const char text1_18[] = "Settings ";
PROGMEM const char text1_19[] = "Bluetooth ";
PROGMEM const char text1_20[] = "WiFi Sniffers ";
PROGMEM const char text1_21[] = "WiFi Attacks ";
PROGMEM const char text1_22[] = "WiFi General ";
PROGMEM const char text1_23[] = "Bluetooth Sniffers ";
PROGMEM const char text1_24[] = "Bluetooth General ";
PROGMEM const char text1_25[] = "Shutdown WiFi ";
PROGMEM const char text1_26[] = "Shutdown BLE ";
PROGMEM const char text1_27[] = "Generate SSIDs ";
PROGMEM const char text1_28[] = "Clear SSIDs ";
PROGMEM const char text1_29[] = "Clear APs ";
PROGMEM const char text1_30[] = "Reboot";
PROGMEM const char text1_31[] = "Sniffers";
PROGMEM const char text1_32[] = "Attacks";
PROGMEM const char text1_33[] = "General";
PROGMEM const char text1_34[] = "Bluetooth Sniffer";
PROGMEM const char text1_35[] = "Detect Card Skimmers";
PROGMEM const char text1_36[] = "Test BadUSB";
PROGMEM const char text1_37[] = "Run Ducky Script";
PROGMEM const char text1_38[] = "Draw";
PROGMEM const char text1_39[] = "Web Update";
PROGMEM const char text1_40[] = "SD Update";
PROGMEM const char text1_41[] = "ESP8266 Update";
PROGMEM const char text1_42[] = "Probe Request Sniff";
PROGMEM const char text1_43[] = "Beacon Sniff";
PROGMEM const char text1_44[] = "Deauth Sniff";
PROGMEM const char text1_45[] = "Packet Monitor";
PROGMEM const char text1_46[] = "EAPOL/PMKID Scan";
PROGMEM const char text1_47[] = "Detect Pwnagotchi";
PROGMEM const char text1_48[] = "Detect Espressif";
PROGMEM const char text1_49[] = "Scan APs";
PROGMEM const char text1_50[] = "Beacon Spam List";
PROGMEM const char text1_51[] = "Beacon Spam Random";
PROGMEM const char text1_52[] = "Rick Roll Beacon";
PROGMEM const char text1_53[] = "Probe Req Flood";
PROGMEM const char text1_54[] = "Deauth Flood";
PROGMEM const char text1_55[] = "Join WiFi";
PROGMEM const char text1_56[] = "Select APs";
PROGMEM const char text1_57[] = "AP Clone Spam";
PROGMEM const char text1_58[] = "Raw Capture";
PROGMEM const char text1_59[] = "Station Sniff";
PROGMEM const char text1_60[] = "Clear Stations";
PROGMEM const char text1_61[] = "Select Stations";
PROGMEM const char text1_62[] = "Deauth Targeted";
PROGMEM const char text1_63[] = "Detect Pineapple";
PROGMEM const char text1_64[] = "Detect MultiSSID";
PROGMEM const char text1_65[] = "Select probe SSIDs";
PROGMEM const char text1_66[] = "GPS"; // Text label for GPS Menu in Main Menu

// SDInterface.cpp texts
PROGMEM const char text2_0[] = "Error, could not find update.bin";
PROGMEM const char text2_1[] = "Starting SD Update...";
PROGMEM const char text2_2[] = "Error, update.bin is empty";
PROGMEM const char text2_3[] = "\nRebooting...\n";
PROGMEM const char text2_4[] = "Could not load update.bin from /";
PROGMEM const char text2_5[] = "File size: ";
PROGMEM const char text2_6[] = "Writing file to partition...";
PROGMEM const char text2_7[] = "Written: ";
PROGMEM const char text2_8[] = "Written only : ";
PROGMEM const char text2_9[] = ". Retry?";
PROGMEM const char text2_10[] = " successfully";
PROGMEM const char text2_11[] = "Update complete";
PROGMEM const char text2_12[] = "Update could not complete";
PROGMEM const char text2_13[] = "Error Occurred. Error #: ";
PROGMEM const char text2_14[] = "Not enough space to begin OTA";

// Web.cpp texts
PROGMEM const char text3_0[] = "Configuring update server...\n\n";
PROGMEM const char text3_1[] = "IP address: ";
PROGMEM const char text3_2[] = "Update: ";
PROGMEM const char text3_3[] = "Bytes complete: ";
PROGMEM const char text3_4[] = "Update Success: ";
PROGMEM const char text3_5[] = "\nCompleted update server setup";

// WiFiScan.cpp texts
PROGMEM const char text4_0[] = " RSSI: ";
PROGMEM const char text4_1[] = "Potential Skimmer: ";
PROGMEM const char text4_2[] = "Already Connected";
PROGMEM const char text4_3[] = "Failed to connect";
PROGMEM const char text4_4[] = "Connected";
PROGMEM const char text4_5[] = "ForcePMKID";
PROGMEM const char text4_6[] = "ForceProbe";
PROGMEM const char text4_7[] = "SavePCAP";
PROGMEM const char text4_8[] = "Probe Flood";
PROGMEM const char text4_9[] = "Clearing APs...";
PROGMEM const char text4_10[] = "APs Cleared: ";
PROGMEM const char text4_11[] = "Clearing SSIDs...";
PROGMEM const char text4_12[] = "SSIDs Cleared: ";
PROGMEM const char text4_13[] = "Generating SSIDs...";
PROGMEM const char text4_14[] = "SSIDs Generated: ";
PROGMEM const char text4_15[] = "    Total SSIDs: ";
PROGMEM const char text4_16[] = "Shutting down WiFi...";
PROGMEM const char text4_17[] = "WiFi not currently initialized";
PROGMEM const char text4_18[] = "Shutting down BLE...";
PROGMEM const char text4_19[] = "BLE not currently initialized";
PROGMEM const char text4_20[] = "Firmware: Marauder";
PROGMEM const char text4_21[] = "Version: ";
PROGMEM const char text4_22[] = "ESP-IDF: ";
PROGMEM const char text4_23[] = "WSL Bypass: enabled";
PROGMEM const char text4_24[] = "WSL Bypass: disabled";
PROGMEM const char text4_25[] = "Station MAC: ";
PROGMEM const char text4_26[] = "AP MAC: ";
PROGMEM const char text4_27[] = "";
PROGMEM const char text4_28[] = "SD Card: Connected";
PROGMEM const char text4_29[] = "SD Card Size: ";
PROGMEM const char text4_30[] = "SD Card: Not Connected";
PROGMEM const char text4_31[] = "SD Card Size: 0";
PROGMEM const char text4_32[] = "IP5306 I2C: supported";
PROGMEM const char text4_33[] = "Battery Lvl: ";
PROGMEM const char text4_34[] = "IP5306 I2C: not supported";
PROGMEM const char text4_35[] = "Internal temp: ";
PROGMEM const char text4_36[] = " Detect Espressif ";
PROGMEM const char text4_37[] = " Detect Pwnagotchi ";
PROGMEM const char text4_38[] = " Beacon Sniffer ";
PROGMEM const char text4_39[] = " Deauthentication Sniffer ";
PROGMEM const char text4_40[] = " Probe Request Sniffer ";
PROGMEM const char text4_41[] = " Bluetooth Sniff ";
PROGMEM const char text4_42[] = " Detect Card Skimmers ";
PROGMEM const char text4_43[] = "Scanning for\nBluetooth-enabled skimmers\nHC-03, HC-05, and HC-06...";
PROGMEM const char text4_44[] = " AP Scan ";
PROGMEM const char text4_45[] = "Clearing Stations...";
PROGMEM const char text4_46[] = "Stations Cleared: ";
PROGMEM const char text4_47[] = "Targeted Deauth";
PROGMEM const char text4_48[] = " Detect Pineapple ";
PROGMEM const char text4_49[] = " Detect MultiSSID ";

// Table declarations (extern only)
extern const char * const text_table0[] PROGMEM;
extern const char * const text_table1[] PROGMEM;
extern const char * const text_table2[] PROGMEM;
extern const char * const text_table3[] PROGMEM;
extern const char * const text_table4[] PROGMEM;

#endif
