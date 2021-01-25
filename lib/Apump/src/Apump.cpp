#include "Apump.h"

Preferences pumpPrefs;
// const int8_t chWaitThreshold = 72;
bool pump[24];
uint16_t ton;
uint16_t toff;
bool phInject = false;
bool phOn = false;

time_t timestamp;
int pumpFilterRelayPin;
int pumpPhRelayPin;
int pumpChRelayPin;
int lightRelayPin;
int countForceDuration = 0;

uint8_t tab[29][24] = {
    // {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //0
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //1
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //2
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //3
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //4
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //5
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //6
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //7
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //8
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
    config.metrics.over_15_duration = pumpPrefs.getShort("chDuration", 0);
    if (config.metrics.over_15_duration >= 10000) // more then 1 year, seems the data have never been initialized correctly
    {
        config.metrics.over_15_duration = 0;
        pumpPrefs.putShort("chDuration", 0);
    }
}

void lightInit(int lightPin)
{

    Serial.print(F("[Relay] Light pin: "));
    Serial.println(lightPin);

    pinMode(lightPin, OUTPUT);
    digitalWrite(lightPin, HIGH);

    lightRelayPin = lightPin;
}

void setLightState(domopool_Config &config)
{

    if (config.global.force_light)
    {
        digitalWrite(lightRelayPin, LOW);
        config.states.light_on = true;
    }
    else
    {
        digitalWrite(lightRelayPin, HIGH);
        config.states.light_on = false;
    }
}

void setFilterState(domopool_Config &config, int hour)
{
    // keep using water temperature if last shown is below 2 degreC
    if (config.states.filter_on || config.metrics.twater <= 2)
    {
        config.metrics.saved_twater = config.metrics.twater;
    }

    int8_t tempWaterAbs = (int)config.metrics.saved_twater;

    bool state;
    if (config.alarms.tw_frost || config.alarms.tamb_frost || config.alarms.tw_high)
    {
        state = true;
    }
    else
    {
        String p = "p";
        p += tempWaterAbs;
        p += hour;
        state = pumpPrefs.getBool(p.c_str(), tab[tempWaterAbs][hour]);
    }

    if (config.alarms.wp_high || (config.sensors.wp.enabled && config.alarms.wp_low && config.pump.automatic))
    {
        state = false;
        config.pump.force_check = true;
    }

    // Start the filter pump if needed
    if (config.metrics.hour != hour || !config.pump.automatic || config.pump.force_check)
    {
        // setting force_check to false in case of automatic toggle (from false to true)
        if (config.pump.force_check)
        {
            unsetForceCheck();
        }

        // keep filter state when swithcing from automatic mode
        if (!config.pump.force_filter && !config.pump.automatic && config.states.automatic)
        {
            config.pump.force_filter = config.states.filter_on;
        }
        config.states.automatic = config.pump.automatic;

        bool chOn = false;
        bool fOn = false;
        // set the pump state based on table calculation or forced
        if (config.pump.automatic)
        {
            fOn = state;
            if (((config.metrics.saved_twater > config.limits.ch_temp_threshold && config.metrics.over_15_duration > config.limits.wait_before_ch) || config.metrics.saved_twater > 18))
            {
                chOn = state;
            }
        }

        if (config.pump.force_filter)
        {
            fOn = true;
            if (config.pump.force_ch)
            {
                chOn = true;
            }
        }

        // set the pump state based on table calculation or forced
        if (fOn)
        {
            Serial.println(F("[Filter] On"));
            config.states.filter_on = true;
            digitalWrite(pumpFilterRelayPin, LOW);
            if (chOn)
            {
                config.states.ch_on = true;
                digitalWrite(pumpChRelayPin, LOW);
            }
            else
            {
                config.states.ch_on = false;
                digitalWrite(pumpChRelayPin, HIGH);
            }
        }
        else
        {
            Serial.println(F("[Filter] Off"));
            digitalWrite(pumpFilterRelayPin, HIGH);
            config.states.filter_on = false;
            config.states.ch_on = false;
            digitalWrite(pumpChRelayPin, HIGH);
        }

        // update chduration only once per hour
        if (config.metrics.hour != hour)
        {
            if (config.metrics.saved_twater > config.limits.ch_temp_threshold && config.metrics.over_15_duration <= config.limits.wait_before_ch)
            {
                config.metrics.over_15_duration++;
                pumpPrefs.putShort("chDuration", config.metrics.over_15_duration);
            }
            if (config.metrics.saved_twater < config.limits.ch_temp_wait_reset && config.metrics.over_15_duration > config.limits.wait_before_ch)
            {
                config.metrics.over_15_duration = 0;
                pumpPrefs.putShort("chDuration", 0);
            }
        }
        if (config.pump.force_filter && config.pump.force_duration != 0)
        {
            uint diff_time = now() - config.pump.force_start_time;
            uint duration_sec = 60 * config.pump.force_duration;
            Serial.print("[pump] Force pump for ");
            Serial.print(duration_sec - diff_time);
            Serial.println("s");
            if (diff_time >= duration_sec)
            {
                Serial.println("[pump] Stoping forced with duration");
                stopRelay(0);
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
    if ((phOn || config.pump.force_ph) && config.states.filter_on)
    {
        // digitalWrite(pumpPhRelayPin, HIGH);
        digitalWrite(pumpPhRelayPin, LOW);
        config.states.ph_on = true;
    }
    else
    {
        digitalWrite(pumpPhRelayPin, HIGH);
        // digitalWrite(pumpPhRelayPin, LOW);
        config.states.ph_on = false;
    }

    Serial.print(F("[PH] Pump state: "));
    Serial.println(config.states.ph_on);
}
