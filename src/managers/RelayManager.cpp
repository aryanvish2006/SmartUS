#include"headers/RelayManager.h"
#include"headers/ConfigManager.h"
#include<LittleFS.h>

#define RELAY_BIN "/relay.bin"
#define RELAY_TMP "/relay.tmp"

static uint8_t relayMask = 0;
static bool relayDirty = false;
static uint32_t lastRelayChange = 0;

Relay relay;

static void saveRelayStatesNow();
static void loadRelayStates();

void Relay::begin(){
    this->isRelayMemoryOn = config.get().isRelayMemoryOn;

    if(isRelayMemoryOn){
        loadRelayStates();
    }else{
        relayMask = 0;
    }

    for(uint8_t i = 0; i<RELAY_COUNT; i++){
        pinMode(relayPins[i],OUTPUT);
        digitalWrite(relayPins[i],getRelayState(i)?LOW:HIGH);
    }
}

void Relay::loop(){
    if(isRelayMemoryOn &&relayDirty && millis() - lastRelayChange > 3000){
        saveRelayStatesNow();
        relayDirty = false;
    }
}

static void loadRelayStates(){
    relayMask = 0 ;

    if(!LittleFS.exists(RELAY_BIN)) return ;

    File f = LittleFS.open(RELAY_BIN,"r");
    if(!f) return ;

    if(f.size() != 1) {
        f.close();
        return;
    }

    relayMask = f.read();
    f.close();
}
static void saveRelayStatesNow(){
    File f = LittleFS.open(RELAY_TMP,"w");
    if(!f) return ;

    f.write(relayMask);
    f.flush();
    f.close();

    LittleFS.remove(RELAY_BIN);
    LittleFS.rename(RELAY_TMP,RELAY_BIN);
}

bool Relay::getRelayState(uint8_t index){
    if(index >= RELAY_COUNT ) return false;
    return (relayMask & ( 1 << index)) != 0;
}
void Relay::setRelayState(uint8_t index, bool on) {
    if(index >= RELAY_COUNT ) return ;

    uint8_t bit = (1 << index);
    bool current = relayMask & bit;

    if(current == on) return;

    if(on) relayMask |= bit;
    else relayMask &= ~bit;

    digitalWrite(relayPins[index],on?LOW:HIGH);

    if(isRelayMemoryOn){
        relayDirty = true;
        lastRelayChange = millis();
    }
}