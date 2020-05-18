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
const int ConfigDocSize = 1024;
struct Network
{
    bool dhcp;
    const char *ip;
    const char *netmask;
    const char *gateway;
    const char *dns;
    bool allowPost;
};
struct Temp
{
    bool enabled;
    bool init;
    uint8_t addr[8];
    float val;
};
struct AnalogSensor
{
    bool enabled;
    float val;
    float threshold;
};

struct Sensors
{
    Temp twin;
    Temp twout;
    Temp tamb;
    bool waitForConversion;
    int tempResolution;
    AnalogSensor ph;
    AnalogSensor ch;
    AnalogSensor waterPressure;
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

struct Metrics
{
    float curTempWater;
    float savedTempWater;
    bool startup;
    bool filterOn;
    bool phOn;
    bool chOn;
    int16_t chDuration;
    int hour;
    Alarms alarms;
};

struct Config
{
    Network network;
    Sensors sensors;
    Global global;
    Time time;
    Pump pump;
    Metrics metrics;
};

void loadConfiguration(const char *filename, Config &config);
bool saveConfiguration(const char *filename, Config &config);
bool saveJson(String &data, Config &config, const char *filename);
void convert2doc(Config &config, JsonDocument &doc);
void convert2config(JsonDocument &doc, Config &config);
void initConfigData(Config &config);
#endif
