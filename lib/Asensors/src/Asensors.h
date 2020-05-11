#include <Aconfig.h>

#include <Arduino.h>
#include <TimeLib.h>
#include <DallasTemperature.h>

#ifndef __ASENSORS_H_UNDEFINED__
#define __ASENSORS_H_UNDEFINED__
bool checkAddress(DeviceAddress deviceAddress, int offset);
void registerDevices(Sensors &config, DallasTemperature &tempSensors);
float roundTemp(float temp);
void showAddressFromEeprom();
void resetEepromSensorsTemp();
#endif
