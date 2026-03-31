#pragma once
#include<Arduino.h>
#define RELAY_COUNT 4
#define RELAY_NAME_LEN 16

struct ConfigData {
    char deviceId[32];
    char username[32];

    char ap_ssid[32];
    char ap_pass[64];

    char sta_ssid[32];
    char sta_pass[64];

    char mqtt_user[32];
    char mqtt_pass[64];

    bool isMqttEnabled;
    bool isStaEnabled;
    bool isApEnabled;
    bool isRelayMemoryOn;

    uint32_t relayDelayMs;

    char relayName[RELAY_COUNT][RELAY_NAME_LEN];

    uint32_t crc;
};

class Config {
    private:
    ConfigData data;
    ConfigData lastSaved;
    bool hasLastSaved = false;
    
    public:
    const ConfigData& get() const { return data;}

    bool begin();
    bool save();
    bool load();
    void loadDefaults();

    bool setApSsidPass(const char* ssid,const char* pass);
    bool setStaSsidPass(const char* ssid,const char* pass);

    bool isApEnabled();
};

extern Config config;
