#include"headers/ConfigManager.h"
#include"headers/RelayManager.h"
#include"headers/WifiManager.h"
void setup(){
    config.begin();
    relay.begin();
    wifi.begin();
}
void loop(){
    relay.loop();
    wifi.loop();
}
