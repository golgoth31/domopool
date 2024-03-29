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

uint8_t tabDynamic[31][24] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 1
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 2
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 3
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 4
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 6
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 7
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 8
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // 10
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // 11
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, // 12
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, // 13
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, // 14
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // 15
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // 16
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // 17
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, // 21
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, // 22
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // 23
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // 24
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // 25
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // 26
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // 27
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, // 28
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, // 29
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 30
};

uint8_t tabHalf[1][24] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // 0
};

uint8_t tabFull[1][24] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0
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
    config.metrics.over_ch_t_high_duration = pumpPrefs.getShort("chDuration", 0);
    if (config.metrics.over_ch_t_high_duration >= 10000) // more then 1 year, seems the data have never been initialized correctly
    {
        config.metrics.over_ch_t_high_duration = 0;
        pumpPrefs.putShort("chDuration", 0);
    }

    config.metrics.saved_twater = pumpPrefs.getFloat("saved_twater", 14);
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
    bool fOn = config.pump.force_filter;
    bool chOn = config.pump.force_ch;
    bool save_temp = false;

    int8_t tempWaterAbs = (int)config.metrics.saved_twater;

    // We set the default state depending on temp and hour and automatic
    if (config.pump.automatic)
    {
        // Enforce filter on alarms or recovering mode
        if (config.alarms.tw_frost || config.alarms.tamb_frost || config.alarms.tw_high || config.pump.recover)
        {
            fOn = true;
        }
        else
        {
            // only change once per hour if needed
            if (config.metrics.hour != hour || config.pump.force_check)
            {
                switch (config.pump.timing)
                {
                case domopool_Pump_timing_half_day:
                    fOn = tabHalf[0][hour];
                    break;
                case domopool_Pump_timing_full_day:
                    fOn = tabFull[0][hour];
                    break;
                default:
                    fOn = tabDynamic[tempWaterAbs][hour];
                    break;
                }

                if (config.pump.force_check)
                {
                    unsetForceCheck();
                }
            }
            else
            {
                fOn = config.states.filter_on;
            }
        }

        // Pressure alarms
        if (config.sensors.wp.enabled && config.alarms.wp_low)
        {
            fOn = false;
            chOn = false;
        }

        if (fOn && !config.states.filter_on)
        {
            if (config.metrics.twater < config.metrics.saved_twater)
            {
                save_temp = true;
            }
            else
            {
                if (config.metrics.saved_twater >= config.limits.ch_temp_threshold_high || config.metrics.twater < config.limits.ch_temp_threshold_high)
                {
                    save_temp = true;
                }
                else
                {
                    pumpPrefs.putUInt("blank_ch", now());
                }
            }
        }

        if (config.metrics.saved_twater >= config.limits.ch_temp_threshold_high)
        {
            chOn = fOn;
        }
        else
        {
            // enforce ch off
            chOn = false;
        }
    }

    // Never start if pressure is high
    // if (config.alarms.wp_high)
    // {
    //     fOn = false;
    //     chOn = false;
    // }

    // Apply states to relay
    if (fOn)
    {
        Serial.println(F("[Filter] On"));
        digitalWrite(pumpFilterRelayPin, LOW);
        config.states.filter_on = true;
        delay(500);
        if (chOn)
        {
            digitalWrite(pumpChRelayPin, LOW);
            config.states.ch_on = true;
        }
        else
        {
            digitalWrite(pumpChRelayPin, HIGH);
            config.states.ch_on = false;
        }

        if (save_temp || (now() - pumpPrefs.getUInt("blank_ch", 0)) > 1800)
        {
            config.metrics.saved_twater = config.metrics.twater;
            save_temp = true;
        }
    }
    else
    {
        Serial.println(F("[Filter] Off"));
        digitalWrite(pumpChRelayPin, HIGH);
        config.states.ch_on = false;
        delay(500);
        digitalWrite(pumpFilterRelayPin, HIGH);
        if (config.states.filter_on)
        {
            pumpPrefs.putFloat("saved_twater", config.metrics.saved_twater);
        }
        config.states.filter_on = false;
        save_temp = false;
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
            Serial.println("[pump] Stopping forced with duration");
            stopRelay(domopool_Relay_names_filter);
        }
    }
    config.metrics.hour = hour;
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

bool forceChDuration(domopool_Config &config)
{
    pumpPrefs.putShort("chDuration", config.limits.ch_wait_before_allow + 1);
    return true;
}
