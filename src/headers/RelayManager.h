#pragma once
#include<Arduino.h>

#define RELAY_COUNT 4

class Relay{
    public:
    void begin();
    void loop();
    bool getRelayState(uint8_t index);
    void setRelayState(uint8_t index, bool on);

    private:
    bool isRelayMemoryOn;

    const uint8_t relayPins[RELAY_COUNT] = {D4,D2,D5,D8};
};

extern Relay relay;