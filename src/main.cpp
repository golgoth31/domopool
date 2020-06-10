// include the dependencies
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
#include <Athings.h>
#include <Adisplay.h>
#include "config.h"

#define ONE_WIRE_BUS 24
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
float tempMoy;

Config config;
const char *filename = "/config.json";

bool serverStarted = false;
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

    pumpInit(filterPin, chPin, phPin);

    initDisplay();

    // Initialize storage
    storageOk = initStorage();

    // if (storageOk)
    // {
    //     tft.setFont(&FreeSansBold24pt7b);
    //     tft.setCursor(30, 65);
    //     tft.print("Storage started");
    // }
    // Should load default config if run for the first time

    Serial.println(F("[Conf] Loading configuration..."));

    loadConfiguration(filename, config);

    // Start up the library

    Serial.println(F("[Sens] Starting..."));

    // start ds18b20 sensors
    tempSensors.begin();

    Serial.println(F("[Sens] Registering addresses..."));

    registerDevices(config.sensors, tempSensors);
    showAddressFromEeprom();

    Serial.println(F("[Sens] Setting sensors options..."));

    tempSensors.setWaitForConversion(config.sensors.waitForConversion);
    tempSensors.setResolution(config.sensors.tempResolution);

    // Start ethernet service

    Serial.println(F("[Eth] Starting server..."));

    serverStarted = startNetwork(ssid, password);

    if (serverStarted)
    {
        Serial.println(F("[Eth] Server is up"));
    }
    else
    {
        Serial.println(F("[Eth] Server not started"));
    }

    initWebThings();

    Serial.println(F("[Time] Setting time..."));

    rtcOk = initSystemTime(config.time);

    Serial.print(F("[Time] Current time: "));
    Serial.println(printTime(true));

    Serial.println(F("[Conf] Updating sensors config..."));

    saveConfiguration(filename, config);

    Serial.println(F("[Conf] Done"));

    initConfigData(config);
    config.metrics.alarms.storage = getStorageAlarm();
    page1(config);
}

void loop(void)
{
    if (millis() - lastReadingTime < 0)
    {
        lastReadingTime = millis();
        lcdBacklightTimer = millis();
    }

    handleWebThings(config);

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

    sendData(config);

    // Get sensors every 2 seconds
    if ((millis() - lastReadingTime) >= 2000)
    {
        tempSensors.requestTemperatures();

        // Serial.print(F("Sensor 'twout' value: "));
        float twout = tempSensors.getTempC(config.sensors.twout.addr);
        // Serial.println(config.sensors.twout.val);
        tempMoy = twout;
        if (config.sensors.twin.enabled)
        {
            // Serial.print(F("Sensor 'twin' value: "));
            float twin = tempSensors.getTempC(config.sensors.twin.addr);
            // Serial.println(config.sensors.twin.val);
            tempMoy = (twout + twin) / 2;
        }
        config.metrics.curTempWater = roundTemp(tempMoy);
        config.metrics.curTempAmbiant = tempSensors.getTempC(config.sensors.tamb.addr);

        displayTemp(config);

        if (!config.metrics.startup)
        {
            filterPumpOn = setFilterState(config, hour());
            if (config.sensors.ph.enabled)
            {
                phPumpOn = setPhState(config, filterPumpOn);
            }
        }
        displayPump(config);
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

        if (config.metrics.startup)
        {

            Serial.println(F("End of startup blanking time"));

            config.metrics.startup = false;
            config.metrics.savedTempWater = config.metrics.curTempWater;
        }

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
