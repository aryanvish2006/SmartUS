#pragma once
#include<Arduino.h>
#include<ESP8266WiFi.h>

class Wifi {
    public:

    void begin();
    void loop();

    bool isConnected() const;
    IPAddress getApIP() const;

    private:

    uint32_t lastAttempt = 0;

    void startAP();
    void stopAP();

    void startSTA();
    void stopSTA();

    void applyConfig();
};

extern Wifi wifi;