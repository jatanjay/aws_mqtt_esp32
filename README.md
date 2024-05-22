# ESP32 AWS IoT Core Onboarding via WiFi / Access Point Mode

## Overview

This project facilitates onboarding ESP32 devices to AWS IoT Core using WiFi or Access Point (AP) mode. It comprises classes: `SpiffsManager`, `WebManager`, `AWSManager`

### SpiffsManager

- Provides APIs for SPIFFS (SPI Flash File System) management.
- Functions:
  - List files present on SPIFFS
  - Check if a file exists
  - Create a new file
  - Write data to a file
  - Erase all data from SPIFFS (use with caution)
  - Read and print contents of a file

### WebManager

- Initializes the web server.
- Facilitates ESP32 operation in AP mode if adding AP fails; otherwise, connects to WiFi.
- Functions:
  - Enters AP mode if adding AP fails
  - Configures ESP32 into AP mode, serves a login page, and sets a custom host name
  - Reads SSID and password from SPIFFS files, adds them to a multi-WiFi configuration
  - Connects ESP32 to WiFi using stored credentials
  - Serves a login page and handles POST requests for updating WiFi credentials

### AWSManager _(WIP)_
- Initializes AWS IoT Core configuration.
- Functions:
  - Initialize AWS IoT Core
  - Connect to AWS IoT Core broker
  - Publish messages to AWS IoT Core
  - Decode base64-encoded payloads
