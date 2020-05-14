#include "Apump.h"

bool pump[24];
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
    Serial.print(F("[Filter] Filter pin: "));
    Serial.println(filterPin);
    pinMode(filterPin, OUTPUT);
    digitalWrite(filterPin, HIGH);

    Serial.print(F("[Filter] PH pin: "));
    Serial.println(phPin);
    pinMode(phPin, OUTPUT);
    digitalWrite(phPin, HIGH);

    pumpFilterRelayPin = filterPin;
    pumpPhRelayPin = phPin;
}

bool setFilterState(Config &config, int hour)
{
    // first disable all
    pumpFullTime(pump, false);

    // keep using water temperature if last chown is below 2 degreC
    if (config.data.filterOn || config.data.curTempWater <= 2)
    {
        config.data.savedTempWater = config.data.curTempWater;
    }

    // enable all if temp too low or to high
    if (config.data.savedTempWater <= 1 || config.data.savedTempWater > 30)
    {
        pumpFullTime(pump, true);
    }

    // put pump on based on temp/hour
    else if (config.data.savedTempWater > 1 && config.data.savedTempWater <= 6)
    {
        for (int i = 3; i <= 4; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.savedTempWater > 6 && config.data.savedTempWater <= 9)
    {
        for (int i = 3; i <= 5; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.savedTempWater > 9 && config.data.savedTempWater <= 12)
    {
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.data.savedTempWater > 12 && config.data.savedTempWater <= 15)
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
    else if (config.data.savedTempWater > 15 && config.data.savedTempWater <= 18)
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
    else if (config.data.savedTempWater > 18 && config.data.savedTempWater <= 21)
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
    else if (config.data.savedTempWater > 21 && config.data.savedTempWater <= 23)
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
    else if (config.data.savedTempWater > 23 && config.data.savedTempWater <= 26)
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
    else if (config.data.savedTempWater > 26 && config.data.savedTempWater <= 28)
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
    else if (config.data.savedTempWater > 28 && config.data.savedTempWater <= 30)
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
    if (config.data.hour != hour)
    {
        if (pump[hour] || config.pump.forceFilter)
        {
            Serial.println(F("[Filter] On"));
            config.data.filterOn = true;
            digitalWrite(pumpFilterRelayPin, LOW);
            // digitalWrite(pumpFilterRelayPin, HIGH);
        }
        else
        {
            Serial.println(F("[Filter] Off"));
            digitalWrite(pumpFilterRelayPin, HIGH);
            // digitalWrite(pumpFilterRelayPin, LOW);
            config.data.filterOn = false;
        }
        config.data.hour = hour;
    }
    Serial.print(F("[Filter] Pump state: "));
    Serial.println(config.data.filterOn);
    return config.data.filterOn;
}

bool setPhState(Config &config, bool filterOn)
{
    // Ph pump have 10m cycles (600s)
    // activate pump for 20% of 10m if ph val is under threshold+0.15
    if ((config.sensors.ph.val <= config.sensors.ph.threshold) && !(phInject))
    {
        ton = 0;
    }
    else if ((config.sensors.ph.val <= (config.sensors.ph.threshold + 0.15)) && !(phInject))
    {
        ton = 120;
        phInject = true;
        timestamp = now();
    }
    // activate pump for 50% of 10m if ph val is under threshold+0.30
    else if ((config.sensors.ph.val <= (config.sensors.ph.threshold + 0.30)) && !(phInject))
    {
        ton = 300;
        phInject = true;
        timestamp = now();
    }
    // activate pump for 75% of 10m if ph val is under threshold+0.45
    else if ((config.sensors.ph.val <= (config.sensors.ph.threshold + 0.45)) && !(phInject))
    {
        ton = 450;
        phInject = true;
        timestamp = now();
    }
    // activate pump for 100% of 10m if ph val is over threshold+0.45
    else if ((config.sensors.ph.val > (config.sensors.ph.threshold + 0.45)) && !(phInject))
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

    // Pilotage Relais Pompe Injection Ph
    if ((phOn || config.pump.forcePH) && filterOn)
    {
        // digitalWrite(pumpPhRelayPin, HIGH);
        digitalWrite(pumpPhRelayPin, LOW);
        config.data.phOn = true;
    }
    else
    {
        digitalWrite(pumpPhRelayPin, HIGH);
        // digitalWrite(pumpPhRelayPin, LOW);
        config.data.phOn = false;
    }
    Serial.print(F("[PH] Pump state: "));
    Serial.println(config.data.phOn);
    return config.data.phOn;
}
