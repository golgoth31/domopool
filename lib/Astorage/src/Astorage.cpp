#include "Astorage.h"

bool initStorage()
{
    if (!SPIFFS.begin(true))
    {

        Serial.println("Card Mount Failed");

        return false;
    }
    return true;
}
// Loads the configuration from a file
File openFile(const char *filename)
{
    SPIFFS.begin(true);
    File file = SPIFFS.open(filename, FILE_WRITE);

    return file;
}

File saveFile(const char *filename)
{
    // Delete existing file, otherwise the configuration is appended to the file
    // char *file2remove;
    // file2remove = const_cast<char *>(filename);
    SPIFFS.begin(true);
    SPIFFS.remove(filename);

    Serial.print(F("[Conf] Opening "));
    Serial.print(filename);
    Serial.println(F("..."));

    // Open file for writing
    File file = SPIFFS.open(filename, FILE_WRITE);

    if (!file)
    {
        Serial.println(F("Failed to create file"));
    }

    return file;
}
