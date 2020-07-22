#include <Arduino.h>

#ifndef __ACONFIG_STRUCT_H__
#define __ACONFIG_STRUCT_H__
struct MQTT
{
    bool enabled = false;
    String server = "192.168.10.194";
};

struct NTP
{
    int dayLight; // 3600 if daylight is observed or 0 if not
    String ntpServer;
    int timeZone; // UTC offset in s: UTC+1=3600
};

struct Network
{
    bool dhcp = true;
    String ip;
    const char *netmask;
    const char *gateway;
    const char *dns;
    bool allowPost = true;
    MQTT mqtt;
    NTP ntp;
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
    int ackDuration;
    double ackTone;
    bool serialOut;
    bool displayStartup;
};

struct Pump
{
    bool forceFilter;
    bool forcePH;
    bool forceCH;
    bool automatic;
    bool forceCheck;
    int forceDuration;
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
    float tempAmbiant;
    float tempWater;
    float savedTempWater;
    float ph;
    float ch;
    float waterPressure;
    int16_t over15Duration;
    int8_t hour;
};

struct States
{
    bool startup;
    bool ntp;
    bool rtc;
    bool filterOn;
    bool phOn;
    bool chOn;
    bool automatic;
    bool net_active;
};

struct Config
{
    Network network;
    Sensors sensors;
    Global global;
    Pump pump;
    Metrics metrics;
    States states;
    Alarms alarms;
    Tests tests;
};
#endif
