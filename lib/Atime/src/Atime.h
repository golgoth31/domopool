#include <Arduino.h>
#include <WiFi.h>
#include <TimeLib.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <domopool.pb.h>

#ifndef __ATIME_H_UNDEFINED__
#define __ATIME_H_UNDEFINED__
void initSystemTime(domopool_Config &config, int sda, int scl);
void setSytemTime(domopool_Config &config);
String printTime(bool seconds);
String printDate();
#endif
