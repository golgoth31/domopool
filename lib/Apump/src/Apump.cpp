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

bool setFilterState(float tempMoy, int hour, bool force)
{
    // first disable all
    Serial.println("[Filter] Setting filter state");
    pumpFullTime(pump, false);

    // enable all if temp too low or to high
    if (tempMoy <= 1 || tempMoy > 30)
    {
        pumpFullTime(pump, true);
    }

    // put pump on based on temp/hour
    else if (tempMoy > 1 && tempMoy <= 6)
    {
        for (int i = 3; i < 5; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 6 && tempMoy <= 9)
    {
        for (int i = 3; i < 6; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 9 && tempMoy <= 12)
    {
        for (int i = 8; i < 12; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 12 && tempMoy <= 15)
    {
        for (int i = 8; i < 11; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i < 16; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 15 && tempMoy <= 18)
    {
        for (int i = 8; i < 11; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i < 17; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 18 && tempMoy <= 21)
    {
        for (int i = 8; i < 12; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i < 17; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 21 && tempMoy <= 23)
    {
        for (int i = 8; i < 12; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i < 18; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 23 && tempMoy <= 26)
    {
        for (int i = 3; i < 5; i++)
        {
            pump[i] = true;
        };
        for (int i = 8; i < 12; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i < 18; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 26 && tempMoy <= 28)
    {
        for (int i = 3; i < 7; i++)
        {
            pump[i] = true;
        };
        for (int i = 8; i < 12; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i < 18; i++)
        {
            pump[i] = true;
        };
    }
    else if (tempMoy > 28 && tempMoy <= 30)
    {
        for (int i = 3; i < 5; i++)
        {
            pump[i] = true;
        };
        for (int i = 8; i < 12; i++)
        {
            pump[i] = true;
        };
        for (int i = 14; i < 20; i++)
        {
            pump[i] = true;
        };
    }

    // Start the filter pump if needed
    if (pump[hour] || force)
    {
        Serial.println("[Filter] On");
        digitalWrite(pumpFilterRelayPin, LOW);
        return true;
    }
    else
    {
        Serial.println("[Filter] Off");
        digitalWrite(pumpFilterRelayPin, HIGH);
        return false;
    }
}

bool setPhState(Ph config, bool force, bool filterOn)
{
    if (config.enabled)
    {
        // Ph pump have 10m cycles (600s)
        // activate pump for 20% of 10m if ph val is under threshold+0.15
        if ((config.val <= config.threshold) && !(phInject))
        {
            ton = 0;
        }
        else if ((config.val <= (config.threshold + 0.15)) && !(phInject))
        {
            ton = 120;
            phInject = true;
            timestamp = now();
        }
        // activate pump for 50% of 10m if ph val is under threshold+0.30
        else if ((config.val <= (config.threshold + 0.30)) && !(phInject))
        {
            ton = 300;
            phInject = true;
            timestamp = now();
        }
        // activate pump for 75% of 10m if ph val is under threshold+0.45
        else if ((config.val <= (config.threshold + 0.45)) && !(phInject))
        {
            ton = 450;
            phInject = true;
            timestamp = now();
        }
        // activate pump for 100% of 10m if ph val is over threshold+0.45
        else if ((config.val > (config.threshold + 0.45)) && !(phInject))
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
    if ((phOn || force) && filterOn)
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
