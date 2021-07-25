#define LARGE_JSON_BUFFERS 1

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
// #include <WebSerial.hpp>
#include <PubSubClient.h>
#include <FS.h>
#include <SPIFFS.h>
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <ArduinoJson/version.hpp>

#include <Aconfig.h>
#include <Asensors.h>
#include <Adisplay.h>

#ifndef __ANETWORK_H_UNDEFINED__
#define __ANETWORK_H_UNDEFINED__
#ifndef DOMOPOOL_VERSION
#define DOMOPOOL_VERSION dev
#endif
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
bool checkIP(const char *ip);
bool startNetwork(const char *ssid, const char *password, domopool_Config &config);
void restartNetwork(const char *ssid, const char *password, domopool_Config &config);
void handleNetwork(domopool_Config &config);
void sendAlarmsMqtt(domopool_Config &config);
void sendMetricsMqtt(domopool_Config &config);
void sendStatesMqtt(domopool_Config &config);
void sendTempsMqtt(domopool_Config &config);
#endif
