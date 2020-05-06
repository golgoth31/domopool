// Local libraries
#include <Aconfig.h>
#include <Aethernet.h>
#include <Apump.h>
#include <Asensors.h>
#include <Atime.h>

// include the dependencies
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
// #include <EthernetUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht.h>
#include <TimeLib.h>

#define ONE_WIRE_BUS 22
#define lcdLEDPin 38
#define lcdLEDButtonPin 34
#define DHTPIN 40
#define pumpFilterPin 48
#define pumpPhPin 49
// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
const int SDCARD_CS_PIN = 4;
const int SDCARD_SS_PIN = 53;

dht dhtClient;

// Ethernet config
EthernetServer server(80);
// EthernetUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// configure timed actions
unsigned long lastReadingTime = 0;
int count_time_30s = 0;   // used to trigger action every 30s (15*2s)
int count_time_30min = 0; // used to trigger action every 30min (60*30s)
int count_time_24h = 0;   // used to trigger action every 24h (2880*30)

// Setup a oneWire instance to communicate with any OneWire devices
OneWire ow(ONE_WIRE_BUS);
// int numTempSensors = 0;
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature tempSensors(&ow);
float tempMoy;

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 9, 8, 7, 6);
int lcdLEDButtonState = 0;
bool lcdLEDBacklightState = true;
unsigned long lcdBacklightTimer = 0;
byte Degree[8] = {
    0b00000,
    0b00100,
    0b01010,
    0b00100,
    0b00000,
    0b00000,
    0b00000,
    0b00000};

Aconfig config;
const char *filename = "config.jsn";

bool serverStarted = false;

bool filterPumpOn = false;
bool phPumpOn = false;

void setup(void)
{
  // start serial port
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pumpInit(pumpFilterPin, pumpPhPin);

  // set up the LCD
  pinMode(lcdLEDPin, OUTPUT);
  pinMode(lcdLEDButtonPin, INPUT_PULLUP);
  digitalWrite(lcdLEDPin, HIGH);
  lcd.begin(16, 2);
  lcd.createChar(0, Degree);
  lcd.clear();
  lcd.setCursor(0, 0);

  Serial.println(F("Starting up"));

  // Initialize SD library
  lcd.print(F("[SD] 1/1"));
  Serial.println(F("[SD] Initializing SD card reader..."));
  pinMode(SDCARD_SS_PIN, OUTPUT);
  while (!SD.begin(SDCARD_CS_PIN))
  {
    Serial.println(F("[SD] Failed to initialize SD library"));
    delay(1000);
  }
  Serial.println(F("[SD] Initialized"));

  // Should load default config if run for the first time
  lcd.setCursor(0, 0);
  lcd.print(F("[Conf] 1/2"));
  Serial.println(F("[Conf] Loading configuration..."));
  loadConfiguration(filename, config);

  // Start up the library
  lcd.setCursor(0, 0);
  lcd.print(F("[Sens] 1/4"));
  Serial.println(F("[Sens] Starting..."));
  // start ds18b20 sensors
  tempSensors.begin();

  // locate devices on the bus
  // lcd.setCursor(0, 0);
  // lcd.print("[Sens] 2/4");
  // Serial.println("[Sens] Locating devices...");
  // Serial.print("[Sens] Found ");
  // numTempSensors = tempSensors.getDeviceCount();
  // Serial.print(numTempSensors, DEC);
  // Serial.println(" devices.");

  lcd.setCursor(0, 0);
  lcd.print(F("[Sens] 3/4"));
  Serial.println(F("[Sens] Registering addresses..."));
  registerDevices(config.sensConfig, tempSensors);

  lcd.setCursor(0, 0);
  lcd.print(F("[Sens] 4/4"));
  Serial.println(F("[Sens] Setting sensors options..."));
  tempSensors.setWaitForConversion(config.sensConfig.waitForConversion);
  tempSensors.setResolution(config.sensConfig.tempResolution);

  // Start ethernet service
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("[Eth] 1/1"));
  Serial.println(F("[Eth] Starting server..."));
  serverStarted = startEthernetServer(config.netConfig);
  if (serverStarted)
  {
    Serial.println(F("[Eth] Server is up"));
  }
  else
  {
    Serial.println(F("[Eth] Server not started"));
  }

  lcd.print(F("[Time] 1/1"));
  Serial.println(F("[Time] Setting time..."));
  initSystemTime(config.time, serverStarted);
  setSyncProvider(RTC.get);
  Serial.print(F("[Time] Current time: "));
  Serial.println(printTime());

  lcd.setCursor(0, 0);
  lcd.print(F("[Conf] 2/2"));
  Serial.println(F("[Conf] Updating sensors config..."));
  saveConfiguration(filename, config);
  Serial.println(F("[Conf] Done"));

  // Setup done, initialize default LCD
  lcd.setCursor(0, 0);
  lcd.print(F("Startup done"));
}

void loop(void)
{
  if (millis() - lastReadingTime < 0)
  {
    lastReadingTime = millis();
    lcdBacklightTimer = millis();
  }

  lcdLEDButtonState = digitalRead(lcdLEDButtonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  // ToDo: add debounce
  if (lcdLEDButtonState == LOW)
  {
    Serial.println(F("[LCD] Ligth ON"));
    digitalWrite(lcdLEDPin, HIGH);
    lcdBacklightTimer = millis();
    lcdLEDBacklightState = true;
  }
  else
  {
    if ((millis() - lcdBacklightTimer) >= config.global.lcdBacklightDuration && lcdLEDBacklightState)
    {
      Serial.println(F("[LCD] Ligth OFF"));
      digitalWrite(lcdLEDPin, LOW);
      lcdLEDBacklightState = false;
    }
  }

  sendData(server, config);

  // Get sensors every 2 seconds
  if ((millis() - lastReadingTime) >= 2000)
  {
    tempSensors.requestTemperatures();
    int chk;
    String dhtError = "No data";
    if (config.sensConfig.tdht.enabled)
    {
      chk = dhtClient.read11(DHTPIN);

      switch (chk)
      {
      case DHTLIB_ERROR_CHECKSUM:
        dhtError = "Checksum error";
        break;
      case DHTLIB_ERROR_TIMEOUT:
        dhtError = "Time out error";
        break;
      case DHTLIB_ERROR_CONNECT:
        dhtError = "Connect error";
        break;
      case DHTLIB_ERROR_ACK_L:
        dhtError = "Ack Low error";
        break;
      case DHTLIB_ERROR_ACK_H:
        dhtError = "Ack High error";
        break;
      default:
        dhtError = "Unknown error";
        break;
      }
    }

    Serial.println(F("[Temp] Printing data..."));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Tout:"));

    Serial.print(F("Sensor 'tout' value: "));
    config.sensConfig.tout.val = tempSensors.getTempC(config.sensConfig.tout.addr);
    Serial.println(config.sensConfig.tout.val);
    lcd.setCursor(6, 0);
    lcd.print(config.sensConfig.tout.val);
    lcd.write(byte(0));
    lcd.print(F("C"));
    if (config.sensConfig.tin.enabled)
    {
      lcd.setCursor(0, 1);
      lcd.print(F("Tin:"));
      Serial.print(F("Sensor 'tin' value: "));
      config.sensConfig.tin.val = tempSensors.getTempC(config.sensConfig.tin.addr);
      Serial.println(config.sensConfig.tin.val);
      lcd.setCursor(5, 1);
      lcd.print(config.sensConfig.tin.val);
      lcd.write(byte(0));
      lcd.print(F("C"));
      tempMoy = (config.sensConfig.tout.val + config.sensConfig.tin.val) / 2;
    }
    else
    {
      tempMoy = config.sensConfig.tout.val;
    }

    if (config.sensConfig.tdht.enabled)
    {
      Serial.print(F("Sensor 'DHT' value: "));
      if (chk == DHTLIB_OK)
      {
        Serial.println(dhtClient.temperature);
      }
      else
      {
        Serial.println(dhtError);
      }
    }

    filterPumpOn = setFilterState(config, hour());
    phPumpOn = setPhState(config, filterPumpOn);

    count_time_30s++; // Count 15 cycles for sending XPL every 30s
    lastReadingTime = millis();
  }

  if (count_time_30s == 15)
  {
    Serial.println(F("*** 30s ***"));
    Serial.print(F("Time: "));
    Serial.println(printTime());

    count_time_30min++; // Count 60 cycles for 30 min
    count_time_30s = 0;
  }
  if (count_time_30min == 60)
  {
    Serial.println(F("*** 30m ***"));
    setSytemTime(serverStarted);
    Serial.print(F("Time: "));
    Serial.println(printTime());
    count_time_30min = 0;
  }
}