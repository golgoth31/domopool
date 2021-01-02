// include the dependencies
#include <Arduino.h>
// #include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <ADS1X15.h>

// Local libraries
#include <Aconfig.h>
#include <Anetwork.h>
#include <Apump.h>
#include <Asensors.h>
#include <Atime.h>
#include <Alarms.h>
#include <Adisplay.h>
#include "config.h"

#define ONE_WIRE_BUS 25
#define filterPin 13
#define phPin 32
#define chPin 33
#define lightPin 16
#define SDA 26
#define SCL 27
#define pressure_adc_pin 3

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
int count_time_10s = 0;   // used to trigger action every 30s (15*2s)
int count_time_30s = 0;   // used to trigger action every 30s (15*2s)
int count_time_30min = 0; // used to trigger action every 30min (60*30s)
int count_time_24h = 0;   // used to trigger action every 24h (2880*30)

// Setup a oneWire instance to communicate with any OneWire devices
OneWire ow(ONE_WIRE_BUS);
DallasTemperature tempSensors(&ow);

// Setup adc
ADS1115 ads(0x48);

domopool_Config config = domopool_Config_init_default;

bool booted = false;

void setup(void)
{
    // start serial port
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    initConfig();
    pumpInit(config, filterPin, chPin, phPin);
    lightInit(lightPin);

    initAlarms();
    initDisplay();

    loadDefaultConfig(config);

    display2boot(F("[Sens] Starting..."), config);

    config.sensors.wp.adc_pin = pressure_adc_pin;
    initializeADS115(config, ads, SDA, SCL);
    // start ds18b20 sensors
    initializeDS18B20(config, tempSensors);

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

    saveConfiguration(config);
    config.tests.enabled = false;
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
    // reboot();
    restartNetwork(ssid, password, config);

    displayPressed(config);
    sendData(config);

    // Get sensors every 2 seconds
    if ((millis() - lastReadingTime) >= 2000)
    {
        pref2config(config);
        setLightState(config);
        getDS18B20(config, tempSensors);
        getWP(config, ads);
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
            int percent = (count_time_10s * 100) / 5;
            displayProgressBar(percent, TFT_DARKCYAN);
        }
        sendMetricsMqtt(config);
        sendStatesMqtt(config);
        count_time_30s++; // Count 15 cycles for sending XPL every 30s
        count_time_10s++; // Count 15 cycles for sending XPL every 30s
        lastReadingTime = millis();
    }

    if (count_time_10s == 5)
    {
        if (config.states.startup)
        {
            Serial.println(F("End of startup blanking time"));
            config.states.startup = false;
            config.metrics.saved_twater = config.metrics.twater;
            displayPageMain(config);
        }
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
        Serial.println(config.metrics.twater);
        Serial.print(F("Sensor 'tamb' value: "));
        Serial.println(config.metrics.tamb);
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
        Serial.println(config.metrics.twater);
        Serial.print(F("Sensor 'tamb' value: "));
        Serial.println(config.metrics.tamb);

        count_time_30min = 0;
    }
}
