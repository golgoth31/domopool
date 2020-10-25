#include <Arduino.h>
// #include <time.h>
#include <TimeLib.h>
#include <Preferences.h>

#include <Aconfig.h>

#ifndef __APUMP_H_UNDEFINED__
#define __APUMP_H_UNDEFINED__
void pumpInit(domopool_Config &config, int filterPin, int chPin, int phPin);
void pumpFullTime(bool pump[24], bool state);
void setFilterState(domopool_Config &config, int hour);
void setPhState(domopool_Config &config);
#endif
