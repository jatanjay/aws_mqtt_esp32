#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

// #include <ESPAsyncWebServer.h>
#include "SpiffsManager.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include "WiFiMulti.h"
#include <Arduino.h>

class WebManager
{
public:
    WebManager(SpiffsManager &spiffs_manager, fs::FS &filesystem);
    void initServer();
    void serveLoginPage();
    bool connectWifi();
    void enterAP();
    bool addAP();
    void setupAPMode();
    void serveOTAPage();

private:


    String setCustomHostName();


    AsyncWebServer server;
    WiFiMulti multiWifi;
    SpiffsManager &spiffs_manager;
    fs::FS &filesystem;
    String MAC_ADDR =  WiFi.macAddress();

    struct Configuration

    {
        String AP_MODE_SSID = "TurnSystemsMgr";
        String AP_MODE_PASS = "turnsys123";
        bool FORCE_FLAG = true;
    };
    Configuration config;
};

#endif
