#include <Aconfig.h>
#include <Asensors.h>

#include <ArduinoJson.h>
#include <WiFi.h>
#include <time.h>

#ifndef __AETHERNET_H_UNDEFINED__
#define __AETHERNET_H_UNDEFINED__

bool checkIP(const char *ip);
bool startNetwork(Network &conf);
void sendData(Config &config);
#endif
