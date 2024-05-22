#include <Arduino.h>
#include <SpiffsManager.h>
#include <WebManager.h>
#include <AWSManager.h>

// fs::FS SPIFFS;


SpiffsManager spiffs_manager;
WebManager web_manager(spiffs_manager, SPIFFS);
AWSManager aws_manager;



void setup()
{
    Serial.begin(115200);
    // spiffs_manager.listFiles();
    web_manager.initServer();
    aws_manager.initAWS();
}


void loop()
{
    // web_manager.handleClient();
}
