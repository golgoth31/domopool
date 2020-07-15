#include <Arduino.h>
#include <time.h>
#include <Preferences.h>

#include <Anetwork.h>

#ifndef __APUMP_H_UNDEFINED__
#define __APUMP_H_UNDEFINED__
void pumpInit(Config &config, int filterPin, int chPin, int phPin);
void pumpFullTime(bool pump[24], bool state);
bool setFilterState(Config &config, int hour);
bool setPhState(Config &config, bool filterOn);
#endif
