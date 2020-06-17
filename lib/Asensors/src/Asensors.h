#include <Aconfig.h>

#include <Arduino.h>
#include <DallasTemperature.h>

#ifndef __ASENSORS_H_UNDEFINED__
#define __ASENSORS_H_UNDEFINED__
bool checkAddress(DeviceAddress deviceAddress, int offset);
void registerDevices(Sensors &config, DallasTemperature &tempSensors);
float roundTemp(float temp);
void showAddressFromEeprom();
void resetEepromSensorsTemp();
void initializeDS18B20(Sensors &config, DallasTemperature &tempSensors);
void getDS18B20(Config &config, DallasTemperature &tempSensors);
#endif
