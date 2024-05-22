#include "AWSManager.h"
#include "WebManager.h"
#include <ArduinoJson.h>
#include "Secrets.h"
AWSManager::AWSManager() 
{
}


bool AWSManager::initAWS(){
    Serial.println(aws_config.SUB_TOPIC);
    Serial.println(aws_config.PUB_TOPIC);
    aws();
    return true;
}



void AWSManager::aws()
{
    // Configure WiFiClientSecure to use the AWS IoT device credentials

    WiFiClientSecure net = WiFiClientSecure();
    PubSubClient client(net);


    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    // Connect to the MQTT broker on the AWS endpoint we defined earlier
    client.setServer(aws_config.IOT_ENDPOINT.c_str(), 8883);

    // Create a message handler

    Serial.println("Connecting to AWS IOT");

    while (!client.connect(aws_config.THING_NAME.c_str()))
    {
        Serial.println(".");
        delay(100);
    }

    if (!client.connected())
    {
        Serial.println("AWS IoT Timeout!");
        return;
    }

    // Subscribe to a topic
    // client.subscribe(aws_iot_sub);
    client.subscribe(aws_config.SUB_TOPIC.c_str());

    Serial.println("AWS IoT Connected!"); // only to test out
    Serial.println("ESP");
    // send_data_aws();


    // StaticJsonDocument<200> doc;

    // doc["PayloadData"] = "MDAwMDAwMDAwMDUzNjU2NWJlNTQzMzRhNGQ0ZDU2MDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMA==";

    // char jsonBuffer[512];
    // serializeJson(doc, jsonBuffer);
    client.publish(aws_config.PUB_TOPIC.c_str(), "Testing Out!");
    Serial.println("publiehsed");


    client.setCallback(messageHandler);
}



void AWSManager::send_data_aws()
{
    // StaticJsonDocument<200> doc;

    // doc["PayloadData"] = "MDAwMDAwMDAwMDUzNjU2NWJlNTQzMzRhNGQ0ZDU2MDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMA==";

    // char jsonBuffer[512];
    // serializeJson(doc, jsonBuffer);
    // client.publish(aws_config.PUB_TOPIC.c_str(), jsonBuffer);
}




// void send_data_aws()
// {
//     StaticJsonDocument<200> doc;

//     doc["PayloadData"] = "MDAwMDAwMDAwMDUzNjU2NWJlNTQzMzRhNGQ0ZDU2MDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMA==";

//     char jsonBuffer[512];
//     serializeJson(doc, jsonBuffer);
//     client.publish(aws_iot_publish, jsonBuffer);
// }



/*

// WiFiClientSecure net = WiFiClientSecure();
// PubSubClient client(net);

// #define AWS_IOT_PUBLISH_TOPIC "QTBin_pub/"
// #define AWS_IOT_SUBSCRIBE_TOPIC "QTBin_sub/"



// const char *thing_name;
// const char *aws_iot_publish;
// const char *aws_iot_sub;

*/







void AWSManager::decoder(const char *basePayload)
{
    size_t base64Length = strlen(basePayload);

    Serial.println("In here!");

    // size_t decodedLength = Base64.decodedLength(const_cast<char *>(basePayload), base64Length);

    // char decodedMessage[decodedLength + 1];

    // Base64.decode(decodedMessage, const_cast<char *>(basePayload), base64Length);

    // decodedMessage[decodedLength] = '\0';

    // Serial.println("Decoded message:");
    // Serial.println(decodedMessage);
    // Serial.println();

    // Serial.println("Converted ASCII:");
    // for (size_t i = 0; i < decodedLength; i += 2)
    // {
    //     char hexPair[3] = {decodedMessage[i], decodedMessage[i + 1], '\0'};
    //     int hexValue = strtol(hexPair, nullptr, 16);
    //     Serial.println(static_cast<char>(hexValue));
    //     if (i + 2 < decodedLength)
    //     {
    //         Serial.println("");
    //     }
    // }
    // Serial.println();
}



void messageHandler(char *topic, byte *payload, unsigned int length)
{
    Serial.println();
    Serial.println("Incoming from topic: ");
    Serial.println(topic);
    Serial.println();

    // StaticJsonDocument<200> doc;
    // deserializeJson(doc, payload);
    // // Serial.println(doc['payload']);
    // const char *base64Message = doc["payload"];
    // // AWSManager::decoder(base64Message);
    Serial.println("in handler");
}


// void aws()
// {
//     // Configure WiFiClientSecure to use the AWS IoT device credentials
//     net.setCACert(AWS_CERT_CA);
//     net.setCertificate(AWS_CERT_CRT);
//     net.setPrivateKey(AWS_CERT_PRIVATE);

//     // Connect to the MQTT broker on the AWS endpoint we defined earlier
//     client.setServer(AWS_IOT_ENDPOINT, 8883);

//     // Create a message handler
//     client.setCallback(messageHandler);

//     Serial.println("Connecting to AWS IOT");

//     while (!client.connect(thing_name))
//     {
//         Serial.println(".");
//         delay(100);
//     }

//     if (!client.connected())
//     {
//         Serial.println("AWS IoT Timeout!");
//         return;
//     }

//     // Subscribe to a topic
//     // client.subscribe(aws_iot_sub);
//     client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

//     Serial.println("AWS IoT Connected!"); // only to test out
//     Serial.println("ESP");
//     send_data_aws();

//     aws_connect = 1;
// }
