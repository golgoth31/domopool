#include "Apump.h"

bool pump[24];
char defaultRelayState = HIGH;
int ton;
int toff;
bool phInject = false;
bool phOn = false;
int timestamp;
int pumpFilterRelayPin;
int pumpPhRelayPin;

void pumpFullTime(bool pump[24], bool state)
{
    for (int i = 0; i < 24; i++)
    {
        pump[i] = state;
    };
}

void pumpInit(int filterPin, int phPin)
{
    pinMode(filterPin, OUTPUT);
    pinMode(phPin, OUTPUT);
    digitalWrite(filterPin, defaultRelayState);
    digitalWrite(phPin, defaultRelayState);
    pumpFilterRelayPin = filterPin;
    pumpPhRelayPin = phPin;
}

bool setFilterState(Aconfig &config, int hour, bool force)
{
    // first disable all
    Serial.println("[Filter] Setting filter state");
    pumpFullTime(pump, false);

    // enable all if temp too low or to high
    if (config.data.curTemp <= 1 || config.data.curTemp > 30)
    {
        pumpFullTime(pump, true);
    }

    // put pump on based on temp/hour
    else if (config.data.curTemp > 1 && config.data.curTemp <= 6)
    {
        for (int i = 3; i <= 4; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 6 && config.data.curTemp <= 9)
    {
        for (int i = 3; i <= 5; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 9 && config.data.curTemp <= 12)
    {
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 12 && config.data.curTemp <= 15)
    {
        for (int i = 8; i <= 10; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i <= 15; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 15 && config.data.curTemp <= 18)
    {
        for (int i = 8; i <= 10; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i <= 16; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 18 && config.data.curTemp <= 21)
    {
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i <= 16; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 21 && config.data.curTemp <= 23)
    {
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i <= 17; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 23 && config.data.curTemp <= 26)
    {
        for (int i = 3; i <= 4; i++)
        {
            pump[i] = true;
        };
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i <= 18; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 26 && config.data.curTemp <= 28)
    {
        for (int i = 3; i <= 5; i++)
        {
            pump[i] = true;
        };
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i <= 19; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.curTemp > 28 && config.data.curTemp <= 30)
    {
        for (int i = 3; i <= 6; i++)
        {
            pump[i] = true;
        };
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i <= 20; i++)
        {
            pump[i] = true;
        };
    }

    // Start the filter pump if needed
    if ((pump[hour] && config.data.hour != hour) || force)
    {
        Serial.println("[Filter] On");
        config.data.hour = hour;
        config.data.filterOn = true;
        digitalWrite(pumpFilterRelayPin, LOW);
        return true;
    }
    else if (!(pump[hour]) && !(force))
    {
        Serial.println("[Filter] Off");
        digitalWrite(pumpFilterRelayPin, HIGH);
        config.data.filterOn = false;
        return false;
    }
}

bool setPhState(Aconfig &config, bool filterOn)
{
    if (config.sensConfig.ph.enabled)
    {
        // Ph pump have 10m cycles (600s)
        // activate pump for 20% of 10m if ph val is under threshold+0.15
        if ((config.sensConfig.ph.val <= config.sensConfig.ph.threshold) && !(phInject))
        {
            ton = 0;
        }
        else if ((config.sensConfig.ph.val <= (config.sensConfig.ph.threshold + 0.15)) && !(phInject))
        {
            ton = 120;
            phInject = true;
            timestamp = now();
        }
        // activate pump for 50% of 10m if ph val is under threshold+0.30
        else if ((config.sensConfig.ph.val <= (config.sensConfig.ph.threshold + 0.30)) && !(phInject))
        {
            ton = 300;
            phInject = true;
            timestamp = now();
        }
        // activate pump for 75% of 10m if ph val is under threshold+0.45
        else if ((config.sensConfig.ph.val <= (config.sensConfig.ph.threshold + 0.45)) && !(phInject))
        {
            ton = 450;
            phInject = true;
            timestamp = now();
        }
        // activate pump for 100% of 10m if ph val is over threshold+0.45
        else if ((config.sensConfig.ph.val > (config.sensConfig.ph.threshold + 0.45)) && !(phInject))
        {
            ton = 600;
            phInject = true;
            timestamp = now();
        }

        if (phInject)
        {
            int curTimestamp = now();
            int deltaTime = curTimestamp - timestamp;
            if (deltaTime < ton)
            {
                phOn = true;
            }
            else
            {
                phOn = false;
            }
            if (deltaTime >= 600)
            {
                phInject = false;
            }
        }
    }
    else
    {
        phOn = false;
    }

    // Pilotage Relais Pompe Injection Ph
    if ((phOn || config.pump.forcePH) && filterOn)
    {
        digitalWrite(pumpPhRelayPin, LOW);
        return true;
    }
    else
    {
        digitalWrite(pumpPhRelayPin, HIGH);
        return false;
    }
}
