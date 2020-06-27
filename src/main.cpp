// include the dependencies
#include <Arduino.h>
#include <Wire.h>
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

#define ONE_WIRE_BUS 17
#define filterPin 48
#define phPin 49
#define chPin 47
#define clkPin 18
// #define dtPin 19

// configure timed actions
unsigned long lastReadingTime = 0;
int count_time_30s = 0;   // used to trigger action every 30s (15*2s)
int count_time_30min = 0; // used to trigger action every 30min (60*30s)
int count_time_24h = 0;   // used to trigger action every 24h (2880*30)

int lcdLEDButtonState = 0;
bool lcdLEDBacklightState = true;
unsigned long lcdBacklightTimer = 0;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire ow(ONE_WIRE_BUS);
DallasTemperature tempSensors(&ow);

Config config;

bool storageOk = true;
bool filterPumpOn = false;
bool phPumpOn = false;
bool rtcOk = false;

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
    displayPageBoot();
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

    // initWebThings();

    Serial.println(F("[Time] Setting time..."));

    rtcOk = initSystemTime(config.time);

    Serial.print(F("[Time] Current time: "));
    Serial.println(printTime(true));

    // saveConfiguration(config);

    initConfigData(config);

    config.tests.enabled = true;
    config.tests.tamb = 25.38;
    config.tests.twater = 25;
    config.tests.pressure = 0.8;

    displayPageMain(config);
}

void loop(void)
{
    if (millis() - lastReadingTime < 0)
    {
        lastReadingTime = millis();
        lcdBacklightTimer = millis();
    }

    // lcdLEDButtonState = digitalRead(lcdLEDButtonPin);
    // // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    // // ToDo: add debounce
    // if (lcdLEDButtonState == LOW)
    // {
    //     Serial.println(F("[LCD] Ligth ON"));
    //     digitalWrite(lcdLEDPin, HIGH);
    //     lcdBacklightTimer = millis();
    //     lcdLEDBacklightState = true;
    // }
    // else
    // {
    //     if ((millis() - lcdBacklightTimer) >= config.global.lcdBacklightDuration && lcdLEDBacklightState)
    //     {
    //         Serial.println(F("[LCD] Ligth OFF"));
    //         digitalWrite(lcdLEDPin, LOW);
    //         lcdLEDBacklightState = false;
    //     }
    // }
    displayPressed(config);
    sendData(config);

    // Get sensors every 2 seconds
    if ((millis() - lastReadingTime) >= 2000)
    {
        pref2config(config);
        displayDate(config);
        displayServices(config);
        getDS18B20(config, tempSensors);

        displayTemp(config);

        if (!config.states.startup)
        {
            filterPumpOn = setFilterState(config, hour());
            if (config.sensors.ph.enabled)
            {
                phPumpOn = setPhState(config, filterPumpOn);
            }
            displayPump(config);
        }
        // else
        // {
        //     displayStartup();
        // }
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
        Serial.println(config.metrics.curTempWater);
        Serial.print(F("Sensor 'tamb' value: "));
        Serial.println(config.metrics.curTempAmbiant);

        if (config.states.startup)
        {
            Serial.println(F("End of startup blanking time"));
            config.states.startup = false;
            config.metrics.savedTempWater = config.metrics.curTempWater;
        }
        // sendMetricsMqtt(config);
        // sendStatesMqtt(config);
        count_time_30min++; // Count 60 cycles for 30 min
        count_time_30s = 0;
    }

    if (count_time_30min == 60)
    {
        setSytemTime(rtcOk);

        Serial.println(F("*** 30m ***"));
        Serial.print(F("Time: "));
        Serial.println(printTime(true));
        Serial.print(F("Sensor 'water' value: "));
        Serial.println(config.metrics.curTempWater);
        Serial.print(F("Sensor 'tamb' value: "));
        Serial.println(config.metrics.curTempAmbiant);

        count_time_30min = 0;
    }
}
