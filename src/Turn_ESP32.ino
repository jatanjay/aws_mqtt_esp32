
// // Author: Jatan Pandya
// // Script to set up aws pub/sub, send uplink and downlink (with base64--hex--ascii conversion)
// // 5/9/24

// #include "secrets.h"
// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <PubSubClient.h>
// #include <ArduinoJson.h>
// #include "WiFiMulti.h"
// #include <ESPAsyncWebServer.h>
// #include <AsyncElegantOTA.h>
// #include <AsyncTCP.h>
// #include "SPIFFS.h"
// #include "time.h"
// #include "Wire.h"
// #include <Base64.h>

// // Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

// WiFiMulti wifiMulti;

// WiFiClientSecure net = WiFiClientSecure();
// PubSubClient client(net);

// #define AWS_IOT_PUBLISH_TOPIC "QTBin_pub/"
// #define AWS_IOT_SUBSCRIBE_TOPIC "QTBin_sub/"

// char header[2];
// char id[4];
// char t[4];
// char bin_id[5];
// uint8_t eve_id;
// char data[13];
// char crc[2];
// char ftr;
// char msg[100];
// uint8_t msg_length;
// uint8_t msg_no = 0;
// uint8_t a = 10;
// bool msg_sel = 0;
// String temp;
// bool wifi_on = 0;
// bool aws_connect = 0;
// bool ap_mode = 0;

// float h;

// // Search for parameter in HTTP POST request
// const char *PARAM_INPUT_1 = "ssid";
// const char *PARAM_INPUT_2 = "pass";

// const uint32_t connectTimeoutMs = 10000;

// // Variables to save values from HTML form
// String ssid, ssid_temp;
// String pass, pass_temp;
// uint8_t ssid_len, pass_len;
// String temp_rssi, ip_addr, ssid_conn;
// String t_n, a_p, a_s;

// const char *thing_name;
// const char *aws_iot_publish;
// const char *aws_iot_sub;

// // File paths to save input values permanently
// const char *ssidPath = "/ssid.txt";
// const char *passPath = "/pass.txt";

// // Timer variables
// unsigned long previousMillis = 0;
// const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)

// String mac_addr;

// // NTP server to request epoch time
// const char *ntpServer = "pool.ntp.org";

// // Variable to save current epoch time*/
// unsigned long epochTime;

// // Function that gets current epoch time
// unsigned long getTime()
// {
//   time_t now;
//   struct tm timeinfo;
//   if (!getLocalTime(&timeinfo))
//   {
//     Serial.println("Failed to obtain time");
//     return (0);
//   }
//   time(&now);
//   return now;
// }

// // Initialize SPIFFS
// void initSPIFFS()
// {
//   if (!SPIFFS.begin(true))
//   {
//     Serial.println("An error has occurred while mounting SPIFFS");
//   }
//   Serial.println("SPIFFS mounted successfully");
// }

// // Read File from SPIFFS
// String readFile(fs::FS &fs, const char *path)
// {
//   Serial.printf("Reading file: %s\r\n", path);

//   File file = fs.open(path);
//   if (!file || file.isDirectory())
//   {
//     Serial.println("- failed to open file for reading");
//     return String();
//   }

//   String fileContent;
//   while (file.available())
//   {
//     // Serial.println(file.readString());
//     fileContent = file.readString();
//     break;
//   }
//   return fileContent;
// }

// // Write file to SPIFFS
// void writeFile(fs::FS &fs, const char *path, const char *message)
// {
//   Serial.printf("Writing file: %s\r\n", path);

//   File file = fs.open(path, FILE_WRITE);
//   if (!file)
//   {
//     Serial.println("- failed to open file for writing");
//     return;
//   }
//   if (file.print(message))
//   {
//     Serial.println("- file written");
//   }
//   else
//   {
//     Serial.println("- write failed");
//   }
// }

// void appendFile(fs::FS &fs, const char *path, const char *message)
// {
//   Serial.printf("Appending to file: %s\r\n", path);

//   File file = fs.open(path, FILE_APPEND);
//   if (!file)
//   {
//     Serial.println(" failed to open file for appending");
//     return;
//   }
//   if (file.print(message))
//   {
//     Serial.println(" message appended");
//   }
//   else
//   {
//     Serial.println(" append failed");
//   }
// }

// // Initialize WiFi
// bool initWiFi()
// {

//   WiFi.mode(WIFI_STA);

//   if (wifiMulti.run() == WL_CONNECTED)
//   {
//     Serial.println(WiFi.SSID());
//     ssid_conn = String(WiFi.SSID());

//     Serial.println(WIFI_SSID);
//     ssid_conn = String(WiFi.SSID());
//     ip_addr = WiFi.localIP().toString();
//     Serial.println(ip_addr);
//     wifi_on = 1;
//     return true;
//   }
//   return false;
// }

// void deleteFile(fs::FS &fs, const char *path)
// {
//   Serial.printf("Deleting file: %s\r\n", path);
//   if (fs.remove(path))
//   {
//     Serial.println("− file deleted");
//   }
//   else
//   {
//     Serial.println("− delete failed");
//   }
// }

// void int_active()
// {
//   digitalWrite(17, HIGH);
// }

// // void send_data_aws()
// // {
// //   StaticJsonDocument<200> doc;
// //   char jsonBuffer[512];
// //   // doc["Mac Address"] = String(WiFi.macAddress());
// //   // doc["Wifi"] = String(WiFi.SSID());
// //   // doc["RSSI"] = String(WiFi.RSSI());
// //   // serializeJson(doc, jsonBuffer);
// //   // serializeJson(doc, Serial);
// //   // Serial.println("");
// //   // client.publish(aws_iot_publish, jsonBuffer);

// //   Serial.println("");
// //   client.publish(aws_iot_publish, jsonBuffer);

// // }

// // test
// void send_data_aws()
// {
//   StaticJsonDocument<200> doc;

//   doc["PayloadData"] = "MDAwMDAwMDAwMDUzNjU2NWJlNTQzMzRhNGQ0ZDU2MDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMA==";

//   char jsonBuffer[512];
//   serializeJson(doc, jsonBuffer);
//   client.publish(aws_iot_publish, jsonBuffer);
// }

// void decoder(const char *basePayload)
// {
//   size_t base64Length = strlen(basePayload);

//   size_t decodedLength = Base64.decodedLength(const_cast<char *>(basePayload), base64Length);

//   char decodedMessage[decodedLength + 1];

//   Base64.decode(decodedMessage, const_cast<char *>(basePayload), base64Length);

//   decodedMessage[decodedLength] = '\0';

//   Serial.println("Decoded message:");
//   Serial.println(decodedMessage);
//   Serial.println();

//   Serial.println("Converted ASCII:");
//   for (size_t i = 0; i < decodedLength; i += 2)
//   {
//     char hexPair[3] = {decodedMessage[i], decodedMessage[i + 1], '\0'};
//     int hexValue = strtol(hexPair, nullptr, 16);
//     Serial.println(static_cast<char>(hexValue));
//     if (i + 2 < decodedLength)
//     {
//       Serial.println("");
//     }
//   }
//   Serial.println();
// };

// void messageHandler(char *topic, byte *payload, unsigned int length)
// {
//   Serial.println();
//   Serial.println("Incoming from topic: ");
//   Serial.println(topic);
//   Serial.println();

//   StaticJsonDocument<200> doc;
//   deserializeJson(doc, payload);
//   const char *base64Message = doc["payload"];
//   decoder(base64Message);
// }

// void aws()
// {
//   // Configure WiFiClientSecure to use the AWS IoT device credentials
//   net.setCACert(AWS_CERT_CA);
//   net.setCertificate(AWS_CERT_CRT);
//   net.setPrivateKey(AWS_CERT_PRIVATE);

//   // Connect to the MQTT broker on the AWS endpoint we defined earlier
//   client.setServer(AWS_IOT_ENDPOINT, 8883);

//   // Create a message handler
//   client.setCallback(messageHandler);

//   Serial.println("Connecting to AWS IOT");

//   while (!client.connect(thing_name))
//   {
//     Serial.println(".");
//     delay(100);
//   }

//   if (!client.connected())
//   {
//     Serial.println("AWS IoT Timeout!");
//     return;
//   }

//   // Subscribe to a topic
//   // client.subscribe(aws_iot_sub);
//   client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

//   Serial.println("AWS IoT Connected!"); // only to test out
//   Serial.println("ESP");
//   send_data_aws();

//   aws_connect = 1;
// }

// void setup()
// {
//   // Serial port for debugging purposes
//   Serial.begin(115200);

//   initSPIFFS();

//   Wire.begin(12, 21, 22, 250000); // join i2c bus with address #8
//   Wire.onReceive(receiveEvent);   // register event
//   Wire.onRequest(requestEvent);
//   // start serial for output

//   Serial.println("I2C started");

//   // Load values saved in SPIFFS

//   // deleteFile(SPIFFS,ssidPath);
//   // deleteFile(SPIFFS,passPath);

//   ssid = readFile(SPIFFS, ssidPath);
//   pass = readFile(SPIFFS, passPath);

//   ssid_len = 0;
//   pass_len = 0;
//   mac_addr = String(WiFi.macAddress());
//   Serial.println(WiFi.macAddress());
//   mac_addr = mac_addr.substring(9);
//   mac_addr = String(mac_addr[0]) + String(mac_addr[1]) + String(mac_addr[3]) + String(mac_addr[4]) + String(mac_addr[6]) + String(mac_addr[7]);

//   t_n = THINGNAME + mac_addr;
//   a_p = AWS_IOT_PUBLISH_TOPIC + mac_addr;
//   a_s = AWS_IOT_SUBSCRIBE_TOPIC + mac_addr;
//   thing_name = t_n.c_str();
//   aws_iot_publish = a_p.c_str();
//   aws_iot_sub = a_s.c_str();
//   Serial.println(mac_addr);
//   wifiMulti.addAP("PepsiMaxStand", "Pepsi2023!!");
//   // wifiMulti.addAP("Verizon_J9SGPG", "cab3-azure-plot"); // Connect directly to your WiFi network

//   while (1)
//   {
//     ssid_len = ssid.indexOf('/');
//     pass_len = pass.indexOf('/');
//     ssid_temp = ssid.substring(0, ssid_len);
//     pass_temp = pass.substring(0, pass_len);
//     Serial.println(ssid_temp);
//     Serial.println(pass_temp);
//     wifiMulti.addAP(ssid_temp.c_str(), pass_temp.c_str());
//     ssid = ssid.substring(ssid_len + 1);
//     pass = pass.substring(pass_len + 1);
//     if (ssid.length() == 0)
//     {
//       break;
//     }
//   }

//   if (initWiFi())
//   {
//     aws();
//     configTime(0, 0, ntpServer);
//     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
//               { request->send(200, "text/plain", "Hi! I am ESP32. I'm successfully connected to AWS!"); });

//     AsyncElegantOTA.begin(&server);
//     server.begin();
//     pinMode(17, OUTPUT);
//     digitalWrite(17, LOW);
//   }
//   else
//   {
//     Serial.println("Setting AP (Access Point)");
//     WiFi.softAP("TurnSystems-WiFiManager", NULL);
//     ap_mode = 1;

//     IPAddress IP = WiFi.softAPIP();
//     Serial.println("AP IP address: ");
//     Serial.println(IP);

//     // Web Server Root URL
//     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
//               { request->send(SPIFFS, "/wifimanager.html", "text/html"); });

//     server.serveStatic("/", SPIFFS, "/");

//     server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
//               {
//     if (request->hasParam("ssid", true) && request->hasParam("pass", true)) {
//     String ssid = request->getParam("ssid", true)->value();
//     String pass = request->getParam("pass", true)->value();

//     Serial.println("Received SSID: ");
//     Serial.println(ssid);
//     ssid += "/";
//     appendFile(SPIFFS, ssidPath, ssid.c_str());

//     Serial.println("Received Password: ");
//     Serial.println(pass);
//     pass += "/";
//     appendFile(SPIFFS, passPath, pass.c_str());
//   } else {
//     request->send(400, "text/plain", "Missing ssid or pass parameters");
//   }
//   delay(3000);
//   ESP.restart(); });
//     server.begin();
//   }
// }

// void receiveEvent(int howMany)
// {
//   StaticJsonDocument<200> doc;
//   char jsonBuffer[512];
//   uint8_t count = 0;
//   while (1 < Wire.available())
//   { // loop through all but the last

//     char c = Wire.read(); // receive byte as a character

//     if (msg_sel)
//     {
//       if (c == 0x72) //r 
//       {
//         msg_no = 0;
//       }
//       else if (c == 0x70) //p
//       {
//         msg_no = 1;
//       }
//       else if (c == 0x71)
//       {
//         msg_no = 2;
//         epochTime = getTime();
//         temp = String(epochTime, HEX);
//         Serial.println(epochTime, HEX);
//       }
//       else if (c == 0x73)
//       {
//         msg_no = 3;
//       }
//       else if (c == 0x74)
//       {
//         msg_no = 4;
//       }
//       else if (c == 0x75)
//       {
//         msg_no = 5;
//       }
//       else if (c == 0x76)
//       {
//         ESP.restart();
//       }
//     }

//     if (c == 0x0F && !count)
//     {
//       // Serial.println("recieve");
//       msg_sel = 1;
//       count++;
//     }
//     else
//     {
//       if (count < 2)
//       {
//         header[count] = c;
//       }
//       else if (count < 5)
//       {
//         id[count - 2] = c;
//       }
//       else if (count < 9)
//       {
//         t[count - 5] = c;
//       }
//       else if (count < 15)
//       {
//         bin_id[count - 9] = c;
//       }
//       else if (count < 16)
//       {
//         eve_id = c;
//       }
//       else if (count < 29)
//       {
//         data[count - 16] = c;
//       }
//       else if (count < 31)
//       {
//         crc[count - 29] = c;
//       }
//       //  Serial.println(c,HEX);         // print the character
//       count++;
//       // Serial.println(count);
//     }
//   }
//   if (!msg_sel)
//   {
//     char time_temp[10];
//     char bin_id_temp[14];
//     char data_temp[30];
//     doc["version"] = "1.0";
//     sprintf(bin_id_temp, "%c%c%c%c%c%c", bin_id[0], bin_id[1], bin_id[2], bin_id[3], bin_id[4], bin_id[5]);
//     doc["bin_id"] = String(bin_id_temp);
//     epochTime = getTime();
//     doc["time"] = String(epochTime, HEX);
//     // sprintf(time_temp,"%x%x%x%x",t[0],t[1],t[2],t[3]);
//     //  doc["time"]    = String(time_temp);
//     doc["event_id"] = String(eve_id);
//     sprintf(data_temp, "%x%x%x%x%x%x%x%x%x%x%x%x%x", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12]);
//     doc["event_data"] = String(data_temp);
//     int x = Wire.read(); // receive byte as an integer
//     ftr = x;
//     // Serial.println(x);         // print the integer
//     serializeJson(doc, jsonBuffer);
//     serializeJson(doc, Serial);
//     Serial.println("");
//     client.publish(aws_iot_publish, jsonBuffer);
//   }
// }

// void requestEvent()
// {
//   Serial.println("request");
//   byte resp[msg_length];
//   byte resp_rssi[23];
//   byte resp_mac[6];
//   byte resp_time[8];
//   int wifi_aws;
//   static uint8_t fnc_call_no = 0;

//   // uint8_t tem = msg_length;
//   if (msg_sel)
//   {
//     switch (msg_no)
//     {
//     case 1:
//       for (int i = 0; i < msg_length; i++)
//       {
//         resp[i] = (byte)msg[i];
//       }
//       Wire.write(resp, msg_length);
//       digitalWrite(17, LOW);
//       break;
//     case 0:
//       Wire.write(msg_length);
//       digitalWrite(17, LOW);
//       break;
//     case 2:
//       Serial.println("t");
//       for (int i = 0; i < 8; i++)
//       {
//         resp_time[i] = temp.charAt(i);
//       }
//       Wire.write(resp_time, 8);
//       break;
//     case 3:
//       wifi_aws = (wifi_on * 10) + aws_connect;
//       Serial.println(wifi_aws);
//       Wire.write(wifi_aws);
//       break;
//     case 4:
//       if (fnc_call_no == 0)
//       {
//         temp_rssi = String(WiFi.RSSI());
//         fnc_call_no++;
//       }
//       else
//       {
//         fnc_call_no = 0;
//       }
//       for (int j = 0; j < 3; j++)
//       {
//         resp_rssi[j] = (byte)temp_rssi[j];
//       }
//       for (int i = 3; i < 8; i++)
//       {
//         resp_rssi[i] = (byte)ssid_conn[i - 3];
//       }
//       for (int k = 8; k < 23; k++)
//       {
//         resp_rssi[k] = (byte)ip_addr[k - 8];
//       }
//       Wire.write(resp_rssi, 23);
//       break;
//     case 5:
//       for (int i = 0; i < 6; i++)
//       {
//         resp_mac[i] = (byte)mac_addr[i];
//       }
//       Wire.write(resp_mac, 6);
//       break;
//     }
//     msg_sel = 0;
//   }
// }

// void loop()
// {
//   if (!ap_mode)
//   {
//     if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED)
//     {
//       if (wifi_on == 0)
//       {
//         ssid_conn = String(WiFi.SSID());
//         ip_addr = WiFi.localIP().toString();
//         wifi_on = 1;
//       }

//       if (aws_connect == 0)
//       {
//         aws();
//       }
//       // Serial.println(WiFi.SSID());
//       client.loop();
//     }
//     else
//     {
//       Serial.println("WiFi not connected!");
//       wifi_on = 0;
//       aws_connect = 0;
//     }
//     delay(1000);
//   }
// }
