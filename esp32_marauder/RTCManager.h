// File: esp32_marauder/RTCManager.h
#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <RTClib.h>
#include <Wire.h>

class RTCManager {
private:
    RTC_DS3231 rtc;
    
public:
    void init();
    String getTimestamp();
    DateTime now();
};

#endif
