#include <Arduino.h>
#include <DallasTemperature.h>
#include <Preferences.h>
#include <domopool.pb.h>
#include <ADS1X15.h>

// #include <Aconfig-struct.h>

#ifndef __ASENSORS_H_UNDEFINED__
#define __ASENSORS_H_UNDEFINED__
bool checkAddress(DeviceAddress deviceAddress, int offset);
void registerDevices(domopool_Config &config, DallasTemperature &tempSensors);
float roundTemp(float temp);
void resetSensorsTempAddr(domopool_Config &config);
void initializeDS18B20(domopool_Config &config, DallasTemperature &tempSensors);
void initializeADS115(domopool_Config &config, ADS1115 &ads, int sda, int scl);
void getDS18B20(domopool_Config &config, DallasTemperature &tempSensors);
void getWP(domopool_Config &config, ADS1115 &ads);
float getWPAnalog(int pin, ADS1115 &ads);
#endif
