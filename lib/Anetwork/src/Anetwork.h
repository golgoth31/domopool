#include <Aconfig.h>
#include <Asensors.h>

#include <ArduinoJson.h>
#include <WiFi.h>
#include <time.h>

#ifndef __AETHERNET_H_UNDEFINED__
#define __AETHERNET_H_UNDEFINED__

bool checkIP(const char *ip);
bool startNetwork(const char *ssid, const char *password);
void sendData(Config &config);
#endif
