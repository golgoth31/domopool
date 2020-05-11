#include "Alarms.h"

#include <EEPROM.h>

// Alarm1 is at offset 24 into eeprom
const int alarm1Offset = 24;
byte alarm1 = 0b00000000;

void setStorageAlarm(bool state)
{
    alarm1 = EEPROM[alarm1Offset];
    if (state)
    {
        bitWrite(alarm1, 0, 1);
    }
    else
    {
        bitWrite(alarm1, 0, 0);
    }
    EEPROM[alarm1Offset] = alarm1;
}
bool getStorageAlarm()
{
    alarm1 = EEPROM[alarm1Offset];
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
