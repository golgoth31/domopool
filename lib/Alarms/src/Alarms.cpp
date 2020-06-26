#include "Alarms.h"

Preferences alarmPrefs;

void initAlarms(bool forceInit)
{
    Serial.println(F("[Alm] Starting..."));
    alarmPrefs.begin("alarms");
    if (alarmPrefs.getBool("init", false) || forceInit)
    {
        Serial.println(F("[Alm] Initializing..."));
        alarmPrefs.putBool("init", true);
        alarmPrefs.putBool("storage", false);
        alarmPrefs.putBool("ofa", false);
    }
    Serial.println(F("[Alm] Done"));
}

void setStorageAlarm(bool state)
{
    alarmPrefs.putBool("storage", state);
}
bool getStorageAlarm()
{
    return alarmPrefs.getBool("storage");
}

// Alarm on ph overtime more then 2h
void setOFAAlarm(bool state)
{
    alarmPrefs.putBool("ofa", state);
}
bool getOFAAlarm()
{
    return alarmPrefs.getBool("ofa");
}
