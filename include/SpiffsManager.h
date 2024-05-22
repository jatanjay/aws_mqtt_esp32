/*
Header file for SPIFFS_MANAGER.C
*/
#ifndef SPIFFS_MANAGER_H
#define SPIFFS_MANAGER_H

#include <FS.h>
#include <SPIFFS.h>

class SpiffsManager
{
public:
    SpiffsManager();
    void listFiles();
    bool fileExists(String file_name);
    bool createFile(String file_name);
    bool openAndWrite(String file_name, String message, String delim);
    void eraseFlash();
    String readFile(String file_name);

private:
    bool SPIFFS_INITIALIZED;
};

#endif
