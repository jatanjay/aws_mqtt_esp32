#ifndef AWS_MANAGER_H
#define AWS_MANAGER_H


#include <WebManager.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Base64.h>

class AWSManager 
{
    public:
    AWSManager();
    bool initAWS();
    void subAWS();
    void pubAWS();
    void send_data_aws();
    // void getUplinkData(); // will come from NRF
    // void setDownlinkData(); // will come from NRF
    // void prepDownlink(); // after getting from nrf
    // void preprUplink(); // after getting from nr

    void prepTopic();
    void aws();
    void decoder(const char *basePayload);

    // void messageHandler(char *topic, byte *payload, unsigned int length);

    private:
    // base64 Base64;
    bool AWS_INITIALIZED;
    struct AWS_CONFIG{
        String THING_NAME = "QTBin";
        String PUB_TOPIC = THING_NAME + "_pub_" + WiFi.macAddress().c_str() + "/";
        String SUB_TOPIC = THING_NAME + "_sub_" + WiFi.macAddress().c_str() + "/";
};
#endif
