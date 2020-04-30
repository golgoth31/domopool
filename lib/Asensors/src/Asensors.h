#include <Aconfig.h>

// #include <Arduino.h>
#include <DallasTemperature.h>

#ifndef __ASENSORS_H_UNDEFINED__
#define __ASENSORS_H_UNDEFINED__
bool checkAddress(DeviceAddress deviceAddress);
Sensors registerDevices(Sensors config, DallasTemperature tempSensors);
#endif
