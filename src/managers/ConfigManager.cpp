#include"headers/ConfigManager.h"
#include<LittleFS.h>

#define CONFIG_BIN "/config.bin"
#define CONFIG_TMP "/config.tmp"

#define VERSION "1.0.0"

Config config;

bool Config::begin(){
    if(!LittleFS.begin()) return false;

    if(load()){
        lastSaved = data;
        hasLastSaved = true;
    }else{
        loadDefaults();
        if(!save()) return false;
    }

    data.isRelayMemoryOn =false;
    data.isApEnabled = true;
    data.isStaEnabled = true;
    strlcpy(data.sta_ssid,"ARYAN NARZO 70 5G",sizeof(data.sta_ssid));
    strlcpy(data.sta_pass,"aryan123",sizeof(data.sta_pass));
    return true;
}

static uint32_t crc32(const uint8_t* data, size_t len){
    uint32_t crc = 0xFFFFFFFF;

    while (len--){
        uint8_t c = *data++;
        for(uint8_t i = 0; i < 8; i++){
            bool bit = ( crc ^ c ) & 1;
            crc >>= 1; 
            if (bit) crc ^= 0xEDB88320;
            c>>=1;            
        }
    }
    return ~crc;
}

bool Config::save(){
    Serial.print("1");
    if(hasLastSaved){
        Serial.print("2");
        ConfigData current = data;
        current.crc = 0;

        ConfigData previous = lastSaved;
        previous.crc = 0;

        if(memcmp(&current,&previous,sizeof(ConfigData))==0){
            Serial.print("3");
            return true;
        }
    }
    Serial.print("4");

    data.crc = 0 ;
    data.crc = crc32((uint8_t*)&data,sizeof(ConfigData));

    File file = LittleFS.open(CONFIG_TMP,"w");
    if(!file)return false;

    size_t written = file.write((uint8_t*)&data,sizeof(ConfigData));

    file.flush();
    file.close();

    if(written != sizeof(ConfigData)){
        LittleFS.remove(CONFIG_TMP);
        return false;
    }
    LittleFS.remove(CONFIG_BIN);
    LittleFS.rename(CONFIG_TMP,CONFIG_BIN);

    lastSaved = data ;
    hasLastSaved = true;
    return true;
}

bool Config::load(){
    if(!LittleFS.exists(CONFIG_BIN)) return false;

    File f = LittleFS.open(CONFIG_BIN,"r");
    if(!f) return false;

    if(f.size() != sizeof(ConfigData)){
        f.close();
        return false;
    }

    f.readBytes((char*)&data,sizeof(ConfigData));
    f.close();

    uint32_t saved = data.crc;
    data.crc = 0;

    uint32_t calc = crc32((uint8_t*)&data,sizeof(ConfigData));

    data.crc = saved;

    return saved == calc;
}

void Config::loadDefaults(){
    memset(&data, 0 , sizeof(ConfigData));

    if(data.deviceId[0]=='\0'){
        snprintf(data.deviceId,sizeof(data.deviceId),"%06X",ESP.getChipId());
    }

    strlcpy(data.ap_ssid,"SMARTUS",sizeof(data.ap_ssid));
    strlcpy(data.ap_pass,"00000000",sizeof(data.ap_pass));

    data.isApEnabled = true;
    data.isMqttEnabled = false;
    data.isStaEnabled = false;
    data.isRelayMemoryOn = false;

    data.relayDelayMs = 150;

    strlcpy(data.relayName[0],"Switch_1",RELAY_NAME_LEN);
    strlcpy(data.relayName[1],"Switch_2",RELAY_NAME_LEN);
    strlcpy(data.relayName[2],"Switch_3",RELAY_NAME_LEN);
    strlcpy(data.relayName[3],"Switch_4",RELAY_NAME_LEN);

    data.crc = 0;
}

bool Config::setApSsidPass(const char* ssid, const char* pass){
    if(!ssid || !pass) return false;

    if(strlen(ssid)==0 || strlen(ssid) >= sizeof(data.ap_ssid)) return false;
    if(strlen(pass) >= sizeof(data.ap_pass)) return false;

    strlcpy(data.ap_ssid,ssid,sizeof(data.ap_ssid));
    strlcpy(data.ap_pass,pass,sizeof(data.ap_pass));

    return save();
}

bool Config::setStaSsidPass(const char* ssid, const char* pass){
    if(!ssid || !pass) return false;

    if(strlen(ssid)==0 || strlen(ssid) >= sizeof(data.sta_ssid)) return false;
    if(strlen(pass) >= sizeof(data.sta_pass)) return false;

    strlcpy(data.sta_ssid,ssid,sizeof(data.sta_ssid));
    strlcpy(data.sta_pass,pass,sizeof(data.sta_pass));

    return save();
}

bool Config::isApEnabled(){
    return data.isApEnabled;
}
