#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <TimeLib.h>
// #include <DS1307RTC.h>
#include <Wire.h>
#include <RtcDS3231.h>

#include <Aconfig-struct.h>

#ifndef __ATIME_H_UNDEFINED__
#define __ATIME_H_UNDEFINED__
void initSystemTime(Config &config, int sda, int scl);
void setSytemTime(bool ethServerStarted, Config &config);
String printTime(bool seconds);
String printDate();
#endif
