#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>

#include <Adisplay.h>
#include <Alarms.h>
#include "Aconfig-struct.h"

#ifndef __ACONFIG_H__
#define __ACONFIG_H__
void loadConfiguration(const char *filename, Config &config);
bool saveConfiguration(const char *filename, Config &config);
bool saveJson(JsonObject json, Config &config, const char *filename);
void config2doc(Config &config, JsonDocument &doc);
void metrics2doc(Config &config, JsonDocument &doc);
void convert2config(JsonDocument &doc, Config &config);
void initConfigData(Config &config);
void object2config(JsonObject doc, Config &config);
#endif
