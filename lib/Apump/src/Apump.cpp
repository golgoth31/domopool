#include "Apump.h"

Preferences pumpPrefs;
// store in eeprom time from temp starts to be upper then 15
// if more then 3 days, activate ch
// int16_t chDuration = 0; // means water temp is over 15°C
const int8_t chWaitThreshold = 72;
bool pump[24];
int ton;
int toff;
bool phInject = false;
bool phOn = false;
bool chOn = false;
time_t timestamp;
int pumpFilterRelayPin;
int pumpPhRelayPin;
int pumpChRelayPin;

void pumpFullTime(bool pump[24], bool state)
{
    for (int i = 0; i < 24; i++)
    {
        pump[i] = state;
    };
}

void pumpInit(Config &config, int filterPin, int chPin, int phPin)
{

    Serial.print(F("[Filter] Filter pin: "));
    Serial.println(filterPin);

    pinMode(filterPin, OUTPUT);
    digitalWrite(filterPin, HIGH);

    Serial.print(F("[Filter] CH pin: "));
    Serial.println(chPin);

    pinMode(chPin, OUTPUT);
    digitalWrite(chPin, HIGH);

    Serial.print(F("[Filter] PH pin: "));
    Serial.println(phPin);

    pinMode(phPin, OUTPUT);
    digitalWrite(phPin, HIGH);

    pumpFilterRelayPin = filterPin;
    pumpPhRelayPin = phPin;
    pumpChRelayPin = chPin;

    pumpPrefs.begin("pump");
    config.metrics.chDuration = pumpPrefs.getShort("chDuration", 0);
    if (config.metrics.chDuration >= 10000) // more then 1 year, seems the data have never been initialized correctly
    {
        config.metrics.chDuration = 0;
        pumpPrefs.putShort("chDuration", 0);
    }
}

bool setFilterState(Config &config, int hour)
{
    // first disable all
    pumpFullTime(pump, false);

    // keep using water temperature if last chown is below 2 degreC
    if (config.states.filterOn || config.metrics.curTempWater <= 2)
    {
        config.metrics.savedTempWater = config.metrics.curTempWater;
    }

    // enable all if temp too low or to high
    if (config.metrics.savedTempWater <= 1 || config.metrics.savedTempWater > 30)
    {
        pumpFullTime(pump, true);
    }

    // put pump on based on temp/hour
    else if (config.metrics.savedTempWater > 1 && config.metrics.savedTempWater <= 6)
    {
        for (int i = 3; i <= 4; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.metrics.savedTempWater > 6 && config.metrics.savedTempWater <= 9)
    {
        for (int i = 3; i <= 5; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.metrics.savedTempWater > 9 && config.metrics.savedTempWater <= 12)
    {
        for (int i = 8; i <= 11; i++)
        {
            pump[i] = true;
        };
    }
    else if (config.metrics.savedTempWater > 12 && config.metrics.savedTempWater <= 15)
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
    else if (config.metrics.savedTempWater > 15 && config.metrics.savedTempWater <= 18)
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
    else if (config.metrics.savedTempWater > 18 && config.metrics.savedTempWater <= 21)
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
    else if (config.metrics.savedTempWater > 21 && config.metrics.savedTempWater <= 23)
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
    else if (config.metrics.savedTempWater > 23 && config.metrics.savedTempWater <= 26)
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
    else if (config.metrics.savedTempWater > 26 && config.metrics.savedTempWater <= 28)
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
    else if (config.metrics.savedTempWater > 28 && config.metrics.savedTempWater <= 30)
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
    if (config.metrics.hour != hour || !config.pump.automatic || config.pump.forceCheck)
    {
        // setting forceCheck to false in case of automatic toggle (from false to true)
        if (config.pump.forceCheck)
        {
            unsetForceCheck();
        }

        // keep filter state when swithcing from automatic mode
        if (!config.pump.forceFilter && !config.pump.automatic && config.states.automatic)
        {
            config.pump.forceFilter = config.states.filterOn;
        }
        config.states.automatic = config.pump.automatic;

        // set the pump state based on table calculation or forced
        if ((pump[hour] && config.pump.automatic) || config.pump.forceFilter)
        {
            Serial.println(F("[Filter] On"));
            config.states.filterOn = true;
            digitalWrite(pumpFilterRelayPin, LOW);
            if ((config.metrics.savedTempWater > 15 && config.metrics.chDuration > chWaitThreshold) || config.metrics.savedTempWater > 18 || config.pump.forceCH)
            {
                config.states.chOn = true;
                digitalWrite(pumpChRelayPin, LOW);
            }
        }
        else
        {
            Serial.println(F("[Filter] Off"));
            digitalWrite(pumpFilterRelayPin, HIGH);
            config.states.filterOn = false;
            config.states.chOn = false;
            digitalWrite(pumpChRelayPin, HIGH);
        }

        // update chduration only once per hour
        if (config.metrics.hour != hour)
        {
            if (config.metrics.savedTempWater > 15 && config.metrics.chDuration <= chWaitThreshold)
            {
                config.metrics.chDuration++;
                pumpPrefs.putShort("chDuration", config.metrics.chDuration);
            }
            else if (config.metrics.savedTempWater < 14 && config.metrics.chDuration > 72)
            {
                config.metrics.chDuration = 0;
                pumpPrefs.putShort("chDuration", 0);
            }
        }
        config.metrics.hour = hour;
    }
    // sendMetricsMqtt(config);
    // sendStatesMqtt(config);
    return config.states.filterOn;
}

bool setPhState(Config &config, bool filterOn)
{
    // Ph pump have 10m cycles (600s)
    // activate pump for 20% of 10m if ph val is under threshold+0.15
    if ((config.metrics.curPh <= config.sensors.ph.threshold) && !(phInject))
    {
        ton = 0;
    }
    else if ((config.metrics.curPh <= (config.sensors.ph.threshold + 0.15)) && !(phInject))
    {
        ton = 120;
        phInject = true;
        time(&timestamp);
    }
    // activate pump for 50% of 10m if ph val is under threshold+0.30
    else if ((config.metrics.curPh <= (config.sensors.ph.threshold + 0.30)) && !(phInject))
    {
        ton = 300;
        phInject = true;
        time(&timestamp);
    }
    // activate pump for 75% of 10m if ph val is under threshold+0.45
    else if ((config.metrics.curPh <= (config.sensors.ph.threshold + 0.45)) && !(phInject))
    {
        ton = 450;
        phInject = true;
        time(&timestamp);
    }
    // activate pump for 100% of 10m if ph val is over threshold+0.45
    else if ((config.metrics.curPh > (config.sensors.ph.threshold + 0.45)) && !(phInject))
    {
        ton = 600;
        phInject = true;
        time(&timestamp);
    }

    if (phInject)
    {
        time_t curTimestamp;
        time(&curTimestamp);
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
        config.states.phOn = true;
    }
    else
    {
        digitalWrite(pumpPhRelayPin, HIGH);
        // digitalWrite(pumpPhRelayPin, LOW);
        config.states.phOn = false;
    }

    Serial.print(F("[PH] Pump state: "));
    Serial.println(config.states.phOn);

    return config.states.phOn;
}
