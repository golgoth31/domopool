#include "Alarms.h"

#include <EEPROM.h>

// Alarm1 is at offset 24 into eeprom
const int alarm1Offset = 40;
byte alarm1 = 0b00000000;

void setStorageAlarm(bool state)
{
    alarm1 = EEPROM.read(alarm1Offset);
    if (state)
    {
        bitWrite(alarm1, 0, 1);
    }
    else
    {
        bitWrite(alarm1, 0, 0);
    }
    EEPROM.write(alarm1Offset, alarm1);
}
bool getStorageAlarm()
{
    alarm1 = EEPROM.read(alarm1Offset);
    int state = bitRead(alarm1, 0);
    if (state == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Alarm on ph overtime more then 2h
void setOFAAlarm(bool state)
{
    alarm1 = EEPROM.read(alarm1Offset);
    if (state)
    {
        bitWrite(alarm1, 1, 1);
    }
    else
    {
        bitWrite(alarm1, 1, 0);
    }
    EEPROM.write(alarm1Offset, alarm1);
}
bool getOFAAlarm()
{
    alarm1 = EEPROM.read(alarm1Offset);
    int state = bitRead(alarm1, 1);
    if (state == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
