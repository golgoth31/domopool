#include <Arduino.h>
#include <DallasTemperature.h>
#include <Preferences.h>

// #include <Aconfig-struct.h>
#include <domopool.pb.h>

#ifndef __ASENSORS_H_UNDEFINED__
#define __ASENSORS_H_UNDEFINED__
bool checkAddress(DeviceAddress deviceAddress, int offset);
void registerDevices(domopool_Config &config, DallasTemperature &tempSensors);
float roundTemp(float temp);
void showAddressFromEeprom();
void resetEepromSensorsTemp();
void initializeDS18B20(domopool_Config &config, DallasTemperature &tempSensors);
void getDS18B20(domopool_Config &config, DallasTemperature &tempSensors);
#endif
