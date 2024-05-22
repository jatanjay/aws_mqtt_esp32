/*
 * File: SpiffsManager.cpp
 * Library for managing SPIFFS (SPI Flash File System) on ESP32 for reading and writing files.
 * Author: Jatan Pandya
 * Date: 5/13/2024
 *
 * Functions:
 * - Initialize SPIFFS
 * - List files present on SPIFFS
 * - Check if a file exists
 * - Create a new file
 * - Write data to a file
 * - Erase all data from SPIFFS (use with caution)
 * - Read and print contents of a file
 */

#include "SpiffsManager.h"

SpiffsManager::SpiffsManager()
{
    SPIFFS_INITIALIZED = SPIFFS.begin(true);
    if (!SPIFFS_INITIALIZED)
    {
        Serial.println("An error occurred while mounting SPIFFS.");
    }
    else
    {
        Serial.println("SPIFFS initialized!");
    }
}

void SpiffsManager::listFiles()
{
    if (!SPIFFS_INITIALIZED)
    {
        Serial.println("SPIFFS not initialized!");
        return;
    }

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    if (!file)
    {
        Serial.println("No files found on SPIFFS.");
    }
    else
    {
        Serial.println("Files currently on SPIFFS:");

        while (file)
        {
            Serial.print("File Name: ");
            Serial.println(file.name());
            file = root.openNextFile();
        }
    }
}

bool SpiffsManager::fileExists(String file_name)
{

    if (SPIFFS.exists("/" + file_name))
    {
        Serial.println("File '" + file_name + "' found on SPIFFS.");
        return true;
    }
    else
    {
        Serial.println("File '" + file_name + "' not found on SPIFFS.");
        return false;
    }
}

bool SpiffsManager::createFile(String file_name)
{
    if (!fileExists(file_name))
    {
        Serial.print("Creating file: ");
        Serial.print(file_name);
        Serial.println("...");
        File file = SPIFFS.open("/" + file_name, "w");
        if (!file)
        {
            Serial.println("Failed to create file: " + file_name);
            return false;
        }
        else
        {
            Serial.println("File created successfully: " + file_name);
            file.close();
            return true;
        }
    }
    else
    {
        Serial.println("File '" + file_name + "' already exists!");
        return false;
    }
}

bool SpiffsManager::openAndWrite(String file_name, String message, String delim)
/*
Note for later, if !bool --> call WIFI.H's ESP.restart();
logic: if file doesn't exist, it is created. Once its' created, the spiff has to be loaded again.
*/
{
    if (!createFile(file_name))
    {
        File file = SPIFFS.open("/" + file_name, "a");
        file.print(message + delim);
        Serial.print("Data written: '");
        Serial.print(message + delim);
        Serial.println("' to file: " + file_name);
        file.close();
        return true;
    }
    else
    {
        createFile(file_name);
        // openAndWrite(file_name, message, delim);
        return false;
    }
}

void SpiffsManager::eraseFlash()
{
    if (SPIFFS.format())
    {
        Serial.println("SPIFFS formatted successfully.");
    }
    else
    {
        Serial.println("SPIFFS format failed.");
    }
}

String SpiffsManager::readFile(String file_name)
{   
    Serial.printf("Reading file: %s\r\n", file_name);

    File file = SPIFFS.open("/" + file_name, "r");
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return String();
    }

    String fileContent;
    while (file.available())
    {
        fileContent = file.readString();
        break;
    }
    return fileContent;
}