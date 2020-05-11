#include <Alarms.h>

#ifdef __AVR_ATmega2560__
#include <Amega2560.h>
#endif

#ifdef ESP32
#include <Aesp32.h>
#endif

#include <Arduino.h>
#include <ArduinoJson.h>

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
    bool init;
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
    Temp twin;
    Temp twout;
    Temp tamb;
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

struct Alarms
{
    bool filter;
    bool ph;
    float phVal;
    bool rtc;
    bool storage;
};

struct Data
{
    float curTempWater;
    float savedTempWater;
    bool startup;
    bool filterOn;
    bool phOn;
    int hour;
    Alarms alarms;
};

struct Aconfig
{
    Network netConfig;
    Sensors sensConfig;
    Global global;
    Time time;
    Pump pump;
    Data data;
};

void loadConfiguration(const char *filename, Aconfig &config);
bool saveConfiguration(const char *filename, Aconfig &config);
bool saveJson(String &data, Aconfig &config, const char *filename);
void convert2doc(Aconfig &config, JsonDocument &doc);
void convert2config(JsonDocument &doc, Aconfig &config);
void initConfigData(Aconfig &config);
#endif
