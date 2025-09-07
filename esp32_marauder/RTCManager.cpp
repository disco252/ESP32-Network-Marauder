// File: esp32_marauder/RTCManager.cpp
#include "RTCManager.h"

void RTCManager::init() {
    Wire.begin(6, 7); // SDA=GPIO6, SCL=GPIO7 for ESP32-C6
    
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        return;
    }
    
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

String RTCManager::getTimestamp() {
    DateTime now = rtc.now();
    char timestamp[32];
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", 
            now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());
    return String(timestamp);
}
