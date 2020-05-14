#include <Aconfig.h>
#include <Asensors.h>

#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef __AVR_ATmega2560__
#include <Amega2560.h>
#include <Ethernet.h>
#include <avr/wdt.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#endif

#include <TimeLib.h>

#ifndef __AETHERNET_H_UNDEFINED__
#define __AETHERNET_H_UNDEFINED__

bool checkIP(const char *ip);
bool startNetwork(Network &conf);
void sendData(Config &config);
#endif
