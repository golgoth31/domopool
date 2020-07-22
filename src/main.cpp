// include the dependencies
#include <Arduino.h>
// #include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>

// Local libraries
#include <Aconfig.h>
#include <Anetwork.h>
#include <Apump.h>
#include <Asensors.h>
#include <Atime.h>
#include <Alarms.h>
#include <Adisplay.h>
#include "config.h"

#define ONE_WIRE_BUS 39
#define filterPin 13
#define phPin 9
#define chPin 10
#define lightPin 11
#define SDA 26
#define SCL 27

// ArduiTouch touch screen pins for TFT_espi
// #define DTFT_MISO 19
// #define DTFT_MOSI 23
// #define DTFT_SCLK 18
// #define DTFT_CS 5
// #define DTFT_DC 4
// #define DTFT_RST 22
// #define DTOUCH_CS 14
// #define DTOUCH_IRQ 2

// configure timed actions
unsigned long lastReadingTime = 0;
int count_time_30s = 0;   // used to trigger action every 30s (15*2s)
int count_time_30min = 0; // used to trigger action every 30min (60*30s)
int count_time_24h = 0;   // used to trigger action every 24h (2880*30)

// Setup a oneWire instance to communicate with any OneWire devices
OneWire ow(ONE_WIRE_BUS);
DallasTemperature tempSensors(&ow);

Config config;

bool booted = false;

void setup(void)
{
    // start serial port
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    pumpInit(config, filterPin, chPin, phPin);

    initAlarms();
    initDisplay();

    loadConfiguration(config);

    display2boot(F("[Sens] Starting..."), config);

    // start ds18b20 sensors
    initializeDS18B20(config.sensors, tempSensors);

    config.states.net_active = startNetwork(ssid, password, config);
    if (config.states.net_active)
    {
        Serial.println(F("[Eth] Network is up"));
    }
    else
    {
        Serial.println(F("[Eth] Network not started"));
    }

    Serial.println(F("[Time] Setting time..."));

    initSystemTime(config, SDA, SCL);

    Serial.print(F("[Time] Current time: "));
    Serial.println(printTime(true));

    initConfigData(config);

    config.tests.enabled = true;
    config.tests.tamb = 25.38;
    config.tests.twater = 25;
    config.tests.pressure = 0.8;
}

void loop(void)
{
    if (millis() - lastReadingTime < 0)
    {
        lastReadingTime = millis();
    }
    restartNetwork(ssid, password, config);

    displayPressed(config);
    sendData(config);

    // Get sensors every 2 seconds
    if ((millis() - lastReadingTime) >= 2000)
    {
        pref2config(config);
        getDS18B20(config, tempSensors);
        if (!config.states.startup)
        {
            displayDate(config);
            displayServices(config);
            displayTemp(config);
            setFilterState(config, hour());
            if (config.sensors.ph.enabled)
            {
                setPhState(config);
            }
            displayPump(config);
        }
        else
        {
            int percent = (count_time_30s * 100) / 15;
            displayProgressBar(percent, TFT_DARKCYAN);
        }
        sendMetricsMqtt(config);
        sendStatesMqtt(config);
        count_time_30s++; // Count 15 cycles for sending XPL every 30s
        lastReadingTime = millis();
    }

    if (count_time_30s == 15)
    {
        Serial.println(F("*** 30s ***"));
        // setSytemTime(rtcOk);
        Serial.print(F("Time: "));
        Serial.println(printTime(true));
        Serial.print(F("Date: "));
        Serial.println(printDate());
        Serial.print(F("Sensor 'water' value: "));
        Serial.println(config.metrics.tempWater);
        Serial.print(F("Sensor 'tamb' value: "));
        Serial.println(config.metrics.tempAmbiant);

        if (config.states.startup)
        {
            Serial.println(F("End of startup blanking time"));
            config.states.startup = false;
            config.metrics.savedTempWater = config.metrics.tempWater;
            displayPageMain(config);
        }
        count_time_30min++; // Count 60 cycles for 30 min
        count_time_30s = 0;
    }

    if (count_time_30min == 60)
    {
        setSytemTime(config.states.rtc, config);

        Serial.println(F("*** 30m ***"));
        Serial.print(F("Time: "));
        Serial.println(printTime(true));
        Serial.print(F("Sensor 'water' value: "));
        Serial.println(config.metrics.tempWater);
        Serial.print(F("Sensor 'tamb' value: "));
        Serial.println(config.metrics.tempAmbiant);

        count_time_30min = 0;
    }
}
