#include "Apump.h"

Preferences pumpPrefs;
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
int countForceDuration = 0;

uint8_t tab[29][24] = {
    // {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //0
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //1
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //2
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //3
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //4
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //5
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //6
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //7
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //8
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //9
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //10
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //11
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //12
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //13
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //14
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //15
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //16
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //17
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //18
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //19
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //20
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //21
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //22
    {0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, //23
    {0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, //24
    {0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, //25
    {0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, //26
    {0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, //27
    {0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, //28
    {0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, //29
    // {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //30
};

void setPrefs(uint8_t t, uint8_t h, uint8_t state)
{
    if (pumpPrefs.getBool("p" + t + h, tab[t][h]) != state)
    {
        pumpPrefs.putBool("p" + t + h, state);
    }
}

void pumpInit(domopool_Config &config, int filterPin, int chPin, int phPin)
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
    config.metrics.over15Duration = pumpPrefs.getShort("chDuration", 0);
    if (config.metrics.over15Duration >= 10000) // more then 1 year, seems the data have never been initialized correctly
    {
        config.metrics.over15Duration = 0;
        pumpPrefs.putShort("chDuration", 0);
    }
}

void setFilterState(domopool_Config &config, int hour)
{
    // keep using water temperature if last shown is below 2 degreC
    if (config.states.filterOn || config.metrics.tempWater <= 2)
    {
        config.metrics.savedTempWater = config.metrics.tempWater;
    }
    uint8_t tempAbs = floor(config.metrics.savedTempWater);
    String p = "p";
    p += tempAbs;
    p += hour;
    // Serial.println(pumpPrefs.getBool(p.c_str(), tab[tempAbs][hour]));
    bool state;
    if (tempAbs < 1 || tempAbs >= 30)
    {
        state = true;
    }
    else
    {
        state = pumpPrefs.getBool(p.c_str(), tab[tempAbs][hour]);
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
        if ((state && config.pump.automatic) || config.pump.forceFilter)
        {
            Serial.println(F("[Filter] On"));
            config.states.filterOn = true;
            digitalWrite(pumpFilterRelayPin, LOW);
            if ((config.metrics.savedTempWater > 15 && config.metrics.over15Duration > chWaitThreshold) || config.metrics.savedTempWater > 18 || config.pump.forceCH)
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
            if (config.metrics.savedTempWater > 15 && config.metrics.over15Duration <= chWaitThreshold)
            {
                config.metrics.over15Duration++;
                pumpPrefs.putShort("chDuration", config.metrics.over15Duration);
            }
            else if (config.metrics.savedTempWater < 14 && config.metrics.over15Duration > 72)
            {
                config.metrics.over15Duration = 0;
                pumpPrefs.putShort("chDuration", 0);
            }
            if (config.pump.forceFilter)
            {
                if (countForceDuration > 0 && config.pump.forceDuration == countForceDuration)
                {
                    config.pump.forceFilter = false;
                    countForceDuration = 0;
                }
                else if (config.pump.forceDuration != 0)
                {
                    countForceDuration++;
                }
            }
        }
        config.metrics.hour = hour;
    }
}

void setPhState(domopool_Config &config)
{
    // Ph pump have 10m cycles (600s)
    // activate pump for 20% of 10m if ph val is under threshold+0.15
    if ((config.metrics.ph <= config.sensors.ph.threshold) && !(phInject))
    {
        ton = 0;
    }
    else if ((config.metrics.ph <= (config.sensors.ph.threshold + 0.15)) && !(phInject))
    {
        ton = 120;
        phInject = true;
        time(&timestamp);
    }
    // activate pump for 50% of 10m if ph val is under threshold+0.30
    else if ((config.metrics.ph <= (config.sensors.ph.threshold + 0.30)) && !(phInject))
    {
        ton = 300;
        phInject = true;
        time(&timestamp);
    }
    // activate pump for 75% of 10m if ph val is under threshold+0.45
    else if ((config.metrics.ph <= (config.sensors.ph.threshold + 0.45)) && !(phInject))
    {
        ton = 450;
        phInject = true;
        time(&timestamp);
    }
    // activate pump for 100% of 10m if ph val is over threshold+0.45
    else if ((config.metrics.ph > (config.sensors.ph.threshold + 0.45)) && !(phInject))
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
    if ((phOn || config.pump.forcePH) && config.states.filterOn)
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
}
