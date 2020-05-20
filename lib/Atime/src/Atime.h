#include <Aconfig.h>

#include <WiFi.h>
#include <time.h>
#include <RTClib.h>

#ifndef __ATIME_H_UNDEFINED__
#define __ATIME_H_UNDEFINED__
void initSystemTime(Time &config, bool ethServerStarted);
void setSytemTime(bool ethServerStarted);
String printTime(bool seconds);
#endif
