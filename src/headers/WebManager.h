#pragma once
#include<Arduino.h>
#include<ESP8266WebServer.h>
#include<LittleFS.h>
#include<DNSServer.h>
class Web {
    private:
    void begin();
    void loop();

    void notify();

    private:
    ESP8266WebServer server{80};
    DNSServer dnsServer;
    WiFiClient sseClient;

    const byte DNS_PORT = 53;

    void handleSSE();
};

extern Web web;