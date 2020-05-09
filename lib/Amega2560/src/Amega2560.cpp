#include "Amega2560.h"

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
#define SDCARD_CS_PIN 4
#define SDCARD_SS_PIN 53

void initStorage()
{
    // Initialize SD library
    Serial.println(F("[SD] Initializing SD card reader..."));
    pinMode(SDCARD_SS_PIN, OUTPUT);
    while (!SD.begin(SDCARD_CS_PIN))
    {
        Serial.println(F("[SD] Failed to initialize SD library"));
        delay(1000);
    }
    Serial.println(F("[SD] Initialized"));
}

// Loads the configuration from a file
File openFile(const char *filename)
{
    File file = SD.open(filename);

    return file;
}

File saveFile(const char *filename)
{
    // Delete existing file, otherwise the configuration is appended to the file
    char *file2remove;
    file2remove = const_cast<char *>(filename);
    SD.remove(file2remove);

    Serial.print(F("[Conf] Opening "));
    Serial.print(filename);
    Serial.println(F("..."));
    // Open file for writing
    File file = SD.open(filename, FILE_WRITE);
    if (!file)
    {
        Serial.println(F("Failed to create file"));
    }

    return file;
}
