#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>

// #include <Adisplay.h>
#include <Alarms.h>
#include "Aconfig-struct.h"

#ifndef __ACONFIG_H__
#define __ACONFIG_H__
void loadConfiguration(Config &config);
void saveConfiguration(Config &config);
void config2doc(Config &config, JsonDocument &doc);
void metrics2doc(Config &config, JsonDocument &doc);
void initConfigData(Config &config);
bool stopPump(const int8_t p);
bool startPump(const int8_t p);
bool setPumpAuto();
bool unsetPumpAuto();
void pref2config(Config &config);
void unsetForceCheck();
void startMqtt();
void stopMqtt();
#endif
