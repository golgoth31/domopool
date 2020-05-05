// #include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

#ifndef __ACONFIG_H_UNDEFINED__
#define __ACONFIG_H_UNDEFINED__
const int AconfigDocSize = 1024;
struct Network
{
    bool dhcp;
    const char *ip;
    const char *netmask;
    const char *gateway;
    const char *dns;
};
struct Temp
{
    bool enabled;
    uint8_t addr[8];
    float val;
};
struct Ph
{
    bool enabled;
    float val;
    float threshold;
};
struct Ch
{
    bool enabled;
    const float *ch;
};
struct Sensors
{
    Temp tin;
    Temp tout;
    Temp tdht;
    bool waitForConversion;
    int tempResolution;
    Ph ph;
    Ch ch;
};
struct Global
{
    unsigned long lcdBacklightDuration;
};

struct Time
{
    bool initialized;
    bool dayLight;
    const char *ntpServer;
    int timeZone;
};
struct Pump
{
    bool forceFilter;
    bool forcePH;
    bool forceCH;
};

struct Config
{
    Network netConfig;
    Sensors sensConfig;
    Global global;
    Time time;
    Pump pump;
};

void loadConfiguration(const char *filename, Config &config);
bool saveConfiguration(const char *filename, Config &config);
bool saveJson(String &data, Config &config, const char *filename);
// StaticJsonDocument<AconfigDocSize> convert2doc(Config &config);
void convert2doc(Config &config, JsonDocument &doc);
void convert2config(JsonDocument &doc, Config &config);
void printFile(const char *filename);
#endif
