#include "Alarms.h"

Preferences alarmPrefs;

void initAlarms(bool forceInit)
{
    display2boot(F("[Alm] Starting..."), true);
    alarmPrefs.begin("alarms");
    if (alarmPrefs.getBool("init", false) || forceInit)
    {
        display2boot(F("[Alm] Initializing..."), true);
        alarmPrefs.putBool("init", true);
        alarmPrefs.putBool("storage", false);
        alarmPrefs.putBool("ofa", false);
    }
    display2boot(F("[Alm] Done"), true);
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
