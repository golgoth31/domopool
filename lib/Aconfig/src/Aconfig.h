#include <Alarms.h>
// #include <Astorage.h>

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#ifndef __ACONFIG_H_UNDEFINED__
#define __ACONFIG_H_UNDEFINED__
const int ConfigDocSize = 2048;

struct MQTT
{
    bool enabled;
    const char *server;
};

struct Network
{
    bool dhcp;
    const char *ip;
    const char *netmask;
    const char *gateway;
    const char *dns;
    bool allowPost;
    MQTT mqtt;
};

struct Temp
{
    bool enabled;
    bool init;
    uint8_t addr[8];
};
struct AnalogSensor
{
    bool enabled;
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
    int dayLight; // 3600 if daylight is observed or 0 if not
    const char *ntpServer;
    int timeZone; // UTC offset in s: UTC+1=3600
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
    bool ch;
    float phVal;
    bool rtc;
    bool storage;
};

struct Tests
{
    bool enabled;
    float twater;
    float tamb;
    float ph;
    float pressure;
};

struct Metrics
{
    float curTempAmbiant;
    float curTempWater;
    float savedTempWater;
    float curPh;
    float curCh;
    float curWaterPressure;
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
    Tests tests;
};

void loadConfiguration(const char *filename, Config &config);
bool saveConfiguration(const char *filename, Config &config);
bool saveJson(JsonObject &json, Config &config, const char *filename);
void config2doc(Config &config, JsonDocument &doc);
void metrics2doc(Config &config, JsonDocument &doc);
void convert2config(JsonDocument &doc, Config &config);
void initConfigData(Config &config);
void object2config(JsonObject doc, Config &config);
#endif
