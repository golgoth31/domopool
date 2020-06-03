#define LARGE_JSON_BUFFERS 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <time.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <Thing.h>
#include <ESPWebThingAdapter.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <Aconfig.h>
#include <Asensors.h>
#include <Athings.h>

#ifndef __ANETWORK_H_UNDEFINED__
#define __ANETWORK_H_UNDEFINED__
bool checkIP(const char *ip);
bool startNetwork(const char *ssid, const char *password);
void sendData(Config &config);
#endif
