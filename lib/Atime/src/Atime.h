#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <TimeLib.h>
// #include <DS1307RTC.h>

#include <Aconfig-struct.h>

#ifndef __ATIME_H_UNDEFINED__
#define __ATIME_H_UNDEFINED__
bool initSystemTime(Time &config);
void setSytemTime(bool ethServerStarted);
String printTime(bool seconds);
String printDate();
#endif
