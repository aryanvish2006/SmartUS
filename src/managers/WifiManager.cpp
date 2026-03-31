#include"headers/WifiManager.h"
#include"headers/ConfigManager.h"
#include<ESP8266WiFi.h>

#include<headers/RelayManager.h>

Wifi wifi;

void Wifi::begin(){
    WiFi.persistent(false);;
    WiFi.setAutoReconnect(false);
    WiFi.mode(WIFI_OFF);

    applyConfig();
}
void Wifi::loop(){

    if(config.get().isStaEnabled && WiFi.status()!=WL_CONNECTED && millis() - lastAttempt > 15000){
        if(strlen(config.get().sta_ssid)>0){
            WiFi.begin(config.get().sta_ssid,config.get().sta_pass);
        }
        lastAttempt = millis();
    }
}

void Wifi::applyConfig(){
    bool ap = config.get().isApEnabled;
    bool sta = config.get().isStaEnabled;

    if(!ap && !sta){
        WiFi.mode(WIFI_OFF);
        return;
    }

    if(ap && sta){
        WiFi.mode(WIFI_AP_STA);
    }else if(ap){
        WiFi.mode(WIFI_AP);
    }else{
        WiFi.mode(WIFI_STA);
    }

    if(ap){
        WiFi.softAP(config.get().ap_ssid,strlen(config.get().ap_pass)?config.get().ap_pass:nullptr);
    }else{
        WiFi.softAPdisconnect(true);
    }

    if(sta){
        if(strlen(config.get().sta_ssid)>0){
            WiFi.begin(config.get().sta_ssid,config.get().sta_pass);
            lastAttempt = millis();
        }
    }else{
        WiFi.disconnect(true);
    }
}

IPAddress Wifi::getApIP() const {
    return WiFi.softAPIP(); 
}