#include <Arduino.h>
#include <DallasTemperature.h>
#include <Preferences.h>
#include <domopool.pb.h>
#include <Adafruit_ADS1015.h>

// #include <Aconfig-struct.h>

#ifndef __ASENSORS_H_UNDEFINED__
#define __ASENSORS_H_UNDEFINED__
bool checkAddress(DeviceAddress deviceAddress, int offset);
void registerDevices(domopool_Config &config, DallasTemperature &tempSensors);
float roundTemp(float temp);
void showAddressFromEeprom();
void resetEepromSensorsTemp();
void initializeDS18B20(domopool_Config &config, DallasTemperature &tempSensors);
void initializeADS115(domopool_Config &config, Adafruit_ADS1115 &ads);
void getDS18B20(domopool_Config &config, DallasTemperature &tempSensors);
void getWP(domopool_Config &config, Adafruit_ADS1115 &ads);
float getWPAnalog(int pin, Adafruit_ADS1115 &ads);
#endif
