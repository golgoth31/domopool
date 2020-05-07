#include <Aconfig.h>

#include <Arduino.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>

#ifdef __AVR_ATmega2560__
#include <EthernetUdp.h>
#endif

#ifdef ESP32
#include <WiFiUdp.h>
#endif

#include <NTPClient.h>

#ifndef __ATIME_H_UNDEFINED__
#define __ATIME_H_UNDEFINED__
void initSystemTime(Time &config, bool ethServerStarted);
void setSytemTime(bool ethServerStarted);
String printTime();
#endif
