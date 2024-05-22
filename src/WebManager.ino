/*
Auth: Jatan Pandya
*/

#include "WebManager.h"

#include "SpiffsManager.h"

WebManager::WebManager(SpiffsManager & spiffs_manager, fs::FS & filesystem): server(80), spiffs_manager(spiffs_manager), filesystem(filesystem) {
  // Constructor fx
}

void WebManager::initServer() {
  enterAP();
}

void WebManager::enterAP() {
  bool addAPSuccess = addAP();

  if (addAPSuccess) {
    if (!connectWifi()) {
 +    }
  } else {
    setupAPMode();
  }
}

void WebManager::setupAPMode() {
  String hostName = setCustomHostName();
  Serial.println("Starting Access Point mode...");
  WiFi.softAP(config.AP_MODE_SSID, config.AP_MODE_PASS);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);
  Serial.println("Access Point mode started.");
  serveLoginPage();
}

bool WebManager::addAP() {

  String ssid_file = "ssid.txt";
  String pass_file = "pass.txt";

  if (!spiffs_manager.fileExists(ssid_file) && !spiffs_manager.fileExists(pass_file)) {
    return false;
  }

  String ssid_content = spiffs_manager.readFile(ssid_file);
  String pass_content = spiffs_manager.readFile(pass_file);

  // Check if either file read failed
  if (ssid_content == nullptr || pass_content == nullptr) {
    Serial.println("Failed to read files");
    return false;
  }

  // Find the index of the first '/' in both ssid and pass content

  Serial.println();
  Serial.println("Existing SSID/Pass found: ");
  Serial.println();

  while (true) {

    int ssid_index = ssid_content.indexOf('/');
    int pass_index = pass_content.indexOf('/');

    // If no '/' found, break the loop
    if (ssid_index == -1 || pass_index == -1) {
      Serial.println("No more SSID/password pairs found");
      break;
    }

    // Extract ssid and pass substrings
    String ssid_temp_buffer = ssid_content.substring(0, ssid_index);
    String pass_temp_buffer = pass_content.substring(0, pass_index);

    // Debug output
    Serial.print("SSID: ");
    Serial.println(ssid_temp_buffer);
    Serial.print("Pass: ");
    Serial.println(pass_temp_buffer);

    // Add AP to multiWifi
    multiWifi.addAP(ssid_temp_buffer.c_str(), pass_temp_buffer.c_str());

    // Update content strings to start from the next index
    ssid_content = ssid_content.substring(ssid_index + 1);
    pass_content = pass_content.substring(pass_index + 1);
  }
  return true;
}

bool WebManager::connectWifi() {

  WiFi.mode(WIFI_STA);

  Serial.print("Connecting to WiFi: ");

  if (multiWifi.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected to: " + WiFi.SSID());
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    serveLoginPage();
    return true;
  } else {
    return false;
  }
}

void WebManager::serveLoginPage() {

  server.on("/", HTTP_GET, [this](AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/wifimanager.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [this](AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/logo.png", HTTP_GET, [this](AsyncWebServerRequest * request) {
    request -> send(SPIFFS, "/logo.png", "text/image");
  });

  server.on("/", HTTP_POST, [this](AsyncWebServerRequest * request) {
    if (request -> hasParam("ssid", true) && request -> hasParam("pass", true)) {
      String ssid_buffer = request -> getParam("ssid", true) -> value();
      String pass_buffer = request -> getParam("pass", true) -> value();
      Serial.println("Received SSID: " + ssid_buffer);
      Serial.println("Received Password: " + pass_buffer);

      bool ssid_success = spiffs_manager.openAndWrite("ssid.txt", ssid_buffer, "/");
      bool pass_success = spiffs_manager.openAndWrite("pass.txt", pass_buffer, "/");

      if (!ssid_success || !pass_success) {
        Serial.println("Failed to write SSID or Password to SPIFFS!");
        request -> send(500, "text/plain", "Internal Server Error");
        return;
      }
      Serial.println("SSID and Password written to SPIFFS successfully!");
      request -> send(200, "text/plain", "Received SSID and Password, ESP32 will now restart!");
      delay(2);
      ESP.restart();
      Serial.println("ESP Restarted!");
    } else {
      request -> send(400, "text/plain", "Missing SSID or Password");
    }
  });
  server.begin();
}

void WebManager::serveOTAPage() {

}

String WebManager::setCustomHostName() {
  MAC_ADDR.replace(":", "");
  MAC_ADDR.toLowerCase();
  int n = 6;
  String last_n_chars = MAC_ADDR.substring(MAC_ADDR.length() - n);
  String custom_host_name = "turnsys_" + last_n_chars;

  return custom_host_name;
}
