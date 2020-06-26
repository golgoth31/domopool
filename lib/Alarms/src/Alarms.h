#include <Arduino.h>
#include <Preferences.h>

// #include <Adisplay.h>

#ifndef __ALARMS_H__
#define __ALARMS_H__
void initAlarms(bool forceInit = false);
void setStorageAlarm(bool state);
bool getStorageAlarm();
#endif
