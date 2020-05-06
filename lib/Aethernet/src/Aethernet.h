#include <Aconfig.h>

#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <avr/wdt.h>
#include <TimeLib.h>

#ifndef __AETHERNET_H_UNDEFINED__
#define __AETHERNET_H_UNDEFINED__
bool checkIP(const char *ip);
bool startEthernetServer(Network conf);
void sendData(EthernetServer server, Aconfig &config);
#endif
