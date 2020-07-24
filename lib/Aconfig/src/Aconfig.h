#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <domopool.pb.h>

#include <Alarms.h>

#ifndef __ACONFIG_H__
#define __ACONFIG_H__
void loadConfiguration(domopool_Config &config);
void saveConfiguration(domopool_Config &config);
void config2doc(domopool_Config &config, JsonDocument &doc);
void metrics2doc(domopool_Config &config, JsonDocument &doc);
void states2doc(domopool_Config &config, JsonDocument &doc);
void initConfigData(domopool_Config &config);
bool stopPump(const int8_t p);
bool startPump(const int8_t p);
bool setPumpAuto();
bool unsetPumpAuto();
void pref2config(domopool_Config &config);
void config2pref(domopool_Config &config);
void unsetForceCheck();
void startMqtt();
void stopMqtt();
void resetConfig();
void reboot();
#endif
