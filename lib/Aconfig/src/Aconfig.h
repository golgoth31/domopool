#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>

// #include <pb_common.h>
// #include <pb_encode.h>
// #include <pb_decode.h>

// #include <Adisplay.h>
#include <Alarms.h>
#include "Aconfig-struct.h"
// #include <domopool.pb.h>

#ifndef __ACONFIG_H__
#define __ACONFIG_H__
void loadConfiguration(Config &config);
void saveConfiguration(Config &config);
void config2doc(Config &config, JsonDocument &doc);
void metrics2doc(Config &config, JsonDocument &doc);
void states2doc(Config &config, JsonDocument &doc);
void initConfigData(Config &config);
bool stopPump(const int8_t p);
bool startPump(const int8_t p);
bool setPumpAuto();
bool unsetPumpAuto();
void pref2config(Config &config);
void config2pref(Config &config);
void unsetForceCheck();
void startMqtt();
void stopMqtt();
#endif
