// Local libraries
#include <Aconfig.h>
#include <Anetwork.h>
#include <Apump.h>
#include <Asensors.h>
#include <Atime.h>
#include <Alarms.h>
#include "config.h"

// include the dependencies
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <SPI.h>
#include <TFT_eSPI.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_ILI9341.h>
// #include <XPT2046_Touchscreen.h>
// #include <Fonts/FreeSansBold9pt7b.h>
// #include <Fonts/FreeSansBold24pt7b.h>

#define ONE_WIRE_BUS 22
#define lcdLEDPin 38
#define lcdLEDButtonPin 34
#define filterPin 48
#define phPin 49
#define chPin 47
#define clkPin 18
#define dtPin 19

/*______Define LCD pins for ArduiTouch _______*/
#define TFT_CS 5
#define TFT_DC 4
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 22
#define TFT_MISO 19
#define TFT_LED 15

#define HAVE_TOUCHPAD
#define TOUCH_CS 14
#define TOUCH_IRQ 2
/*_______End of defanitions______*/

/*______Assign pressure_______*/
#define ILI9341_ULTRA_DARKGREY 0x632C
#define MINPRESSURE 10
#define MAXPRESSURE 2000
/*_______Assigned______*/

/*____Calibrate TFT LCD_____*/
#define TS_MINX 370
#define TS_MINY 470
#define TS_MAXX 3700
#define TS_MAXY 3600
/*______End of Calibration______*/

// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
TFT_eSPI tft = TFT_eSPI();

// #define _debug
int X, Y;

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

    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, LOW);
    tft.init();

    delay(2000);
    Serial.println(F("Starting up"));

    tft.fillScreen(TFT_BLACK);

    // Set "cursor" at top left corner of display (0,0) and select font 4
    tft.setCursor(0, 0, 4);

    // Set the font colour to be white with a black background
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // We can now plot text on screen using the "print" class
    tft.println("Intialised default\n");
    tft.println("White text");

    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Red text");

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Green text");

    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.println("Blue text");

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

    Serial.println(F("[Time] Setting time..."));
    rtcOk = initSystemTime(config.time);
    Serial.print(F("[Time] Current time: "));
    Serial.println(printTime(true));

    Serial.println(F("[Conf] Updating sensors config..."));
    saveConfiguration(filename, config);
    Serial.println(F("[Conf] Done"));

    initConfigData(config);
    config.metrics.alarms.storage = getStorageAlarm();
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

    sendData(config);

    // Get sensors every 2 seconds
    if ((millis() - lastReadingTime) >= 2000)
    {
        tempSensors.requestTemperatures();

        // Serial.print(F("Sensor 'twout' value: "));
        config.sensors.twout.val = tempSensors.getTempC(config.sensors.twout.addr);
        // Serial.println(config.sensors.twout.val);
        tempMoy = config.sensors.twout.val;
        if (config.sensors.twin.enabled)
        {
            // Serial.print(F("Sensor 'twin' value: "));
            config.sensors.twin.val = tempSensors.getTempC(config.sensors.twin.addr);
            // Serial.println(config.sensors.twin.val);
            tempMoy = (config.sensors.twout.val + config.sensors.twin.val) / 2;
        }
        config.metrics.curTempWater = roundTemp(tempMoy);
        Serial.print(F("Sensor water value: "));
        Serial.println(config.metrics.curTempWater);

        Serial.print(F("Sensor 'tamb' value: "));
        config.sensors.tamb.val = tempSensors.getTempC(config.sensors.tamb.addr);
        Serial.println(config.sensors.tamb.val);

        if (!config.metrics.startup)
        {
            filterPumpOn = setFilterState(config, hour());
            if (config.sensors.ph.enabled)
            {
                phPumpOn = setPhState(config, filterPumpOn);
            }
        }

        count_time_30s++; // Count 15 cycles for sending XPL every 30s
        lastReadingTime = millis();
    }

    if (count_time_30s == 15)
    {
        Serial.println(F("*** 30s ***"));
        // setSytemTime(rtcOk);
        Serial.print(F("Time: "));
        Serial.println(printTime(true));
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
        Serial.println(F("*** 30m ***"));
        setSytemTime(rtcOk);
        Serial.print(F("Time: "));
        Serial.println(printTime(true));
        count_time_30min = 0;
    }
}
