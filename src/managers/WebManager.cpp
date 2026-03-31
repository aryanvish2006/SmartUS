#include"headers/WebManager.h"
#include"headers/ConfigManager.h"
#include"headers/WifiManager.h"
Web web;

void Web::begin(){
    server.serveStatic("/",LittleFS,"/index.html");
    server.serveStatic("/style.css",LittleFS,"/style.css");
    server.serveStatic("/communication.js",LittleFS,"/communication.js");
    server.serveStatic("/ui.js",LittleFS,"/ui.js");
    server.serveStatic("/logic.js",LittleFS,"/logic.js");

    server.on("/events",std::bind(&Web::handleSSE,this));

    server.onNotFound([this](){
        server.sendHeader("Location","/",true);
        server.send(302,"text/plain","");
    });

    server.begin();

    if(config.isApEnabled()){
        dnsServer.start(DNS_PORT,"*",wifi.getApIP());
    }
}
void Web::loop(){
    if(config.isApEnabled()){
        dnsServer.processNextRequest();
    }
    server.handleClient();
}
void Web::handleSSE(){
    WiFiClient client = server.client();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/event-stream");
    client.println("Cache-Control: no-cache");
    client.println("Connection: keep-alive");
    client.println();

    sseClient = client;
}
