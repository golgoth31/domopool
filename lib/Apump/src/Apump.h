#include <Aconfig.h>

#ifdef __AVR_ATmega2560__
#include <Amega2560.h>
#endif

#include <Arduino.h>
#include <TimeLib.h>

#ifndef __APUMP_H_UNDEFINED__
#define __APUMP_H_UNDEFINED__
void pumpInit(int filterPin, int phPin);
void pumpFullTime(bool pump[24], bool state);
bool setFilterState(Aconfig &config, int hour);
bool setPhState(Aconfig &config, bool filterOn);
#endif
