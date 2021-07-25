#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <TimeLib.h>
#include <domopool.pb.h>

#ifndef __ACONFIG_H__
#define __ACONFIG_H__
bool initConfig();
void loadDefaultConfig(domopool_Config &config);
void saveConfiguration(domopool_Config &config);
void initConfigData(domopool_Config &config);
bool stopRelay(const int8_t p);
bool startRelay(const int8_t p, uint32_t duration);
void toggleRelayAuto(domopool_Config &config);
// void setRelayAuto(domopool_Config &config);
// void unsetRelayAuto();
// void unsetRelayAutoRecover();
// void setRelayAutoRecover(domopool_Config &config);
void toggleRelayAutoRecover(domopool_Config &config);
void pref2config(domopool_Config &config);
void config2pref(domopool_Config &config);
void unsetForceCheck();
void setForceCheck();
void startMqtt();
void stopMqtt();
void setMqtt(String server);
void resetConfig();
void reboot();
void setWP(domopool_AnalogSensor &analog);
void enableWP();
void disableWP();
void setPH(domopool_AnalogSensor &analog);
void enablePH();
void disablePH();
void setCH(domopool_AnalogSensor &analog);
void enableCH();
void disableCH();
void setADC(domopool_Sensors &sens);
void setLimits(domopool_Limits &limits);
void alarms2doc(domopool_Config &config, JsonDocument &doc);
void metrics2doc(domopool_Config &config, JsonDocument &doc);
void states2doc(domopool_Config &config, JsonDocument &doc);
void temps2doc(domopool_Config &config, JsonDocument &doc);
#endif
