// File: esp32_marauder/EthernetDiagnostics.h
#ifndef ETHERNET_DIAGNOSTICS_H
#define ETHERNET_DIAGNOSTICS_H

#include <Arduino.h>
#include <ETH.h>
#include <WiFi.h>

class EthernetDiagnostics {
private:
    bool ethConnected;
    IPAddress localIP;
    IPAddress gateway;

public:
    void init();
    void scanDHCPServers();
    void performTraceroute(IPAddress target);
    void pingTest(IPAddress target, int count);
    void networkScan();
    bool isConnected();
};

#endif
