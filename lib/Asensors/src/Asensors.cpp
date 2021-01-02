#include "Asensors.h"

Preferences sensPrefs;

String twout = "twout";
String twin = "twin";
String tamb = "tamb";

bool checkAddress(DeviceAddress deviceAddress, String temp)
{
    bool ret = true;
    for (uint8_t i = 0; i < 8; i++)
    {
        // Serial.print(F("Char "));
        // Serial.print(i);
        // Serial.print(" from prefs: ");
        // Serial.print(sensPrefs.getUChar(temp + i, 0));
        // Serial.println();

        // Serial.print(F("Char "));
        // Serial.print(i);
        // Serial.print(" from device: ");
        // Serial.print(deviceAddress[i]);
        // Serial.println();
        temp.concat(i);
        if (sensPrefs.getUChar(temp.c_str(), 0) == deviceAddress[i])
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    Serial.print(F("Known "));
    Serial.print(temp);
    Serial.print(": ");
    Serial.print(ret);
    Serial.println();
    return ret;
}

void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Serial.print(F("0x"));
        if (deviceAddress[i] < 0x10)
        {
            Serial.print(F("0"));
        }
        Serial.print(deviceAddress[i], HEX);
        if (i < 7)
        {
            Serial.print(F(", "));
        }
    }
}

void printAddressFromPref(String temp)
{

    for (uint8_t i = 0; i < 8; i++)
    {
        temp.concat(i);
        Serial.print(F("0x"));
        if (sensPrefs.getUChar(temp.c_str(), 0) < 0x10)
        {
            Serial.print(F("0"));
        }
        Serial.print(sensPrefs.getUChar(temp.c_str(), 0), HEX);
        if (i < 7)
        {
            Serial.print(F(", "));
        }
    }
}

void registerDevices(domopool_Config &config, DallasTemperature &tempSensors)
{
    DeviceAddress deviceAddress;
    int num = tempSensors.getDeviceCount();

    Serial.print(F("[Sens] Found "));
    Serial.print(num);
    Serial.println(F(" sensors"));

    bool twout_bool = false;
    bool tamb_bool = false;
    bool twin_bool = false;

    for (int j = 0; j < num; j++)
    {

        bool devOk = tempSensors.getAddress(deviceAddress, j);
        if (devOk)
        {
            Serial.print(F("[Sens] Found ds18b20 address: "));
            printAddress(deviceAddress);
            Serial.println();

            twout_bool = checkAddress(deviceAddress, twout);
            tamb_bool = checkAddress(deviceAddress, tamb);
            if (config.sensors.twin.enabled)
            {
                twin_bool = checkAddress(deviceAddress, twin);
            }

            if (twout_bool)
            {
                for (uint8_t i = 0; i < 8; i++)
                {
                    config.sensors.twout.addr[i] = deviceAddress[i];
                }
                continue;
            }
            else if (!config.sensors.twout.init && !tamb_bool && !twin_bool)
            {

                Serial.print(F("[Sens] twout read address: "));
                String temp_str = twout;
                for (uint8_t i = 0; i < 8; i++)
                {
                    temp_str.concat(i);
                    sensPrefs.putUChar(temp_str.c_str(), deviceAddress[i]);
                    config.sensors.twout.addr[i] = deviceAddress[i];
                    config.sensors.twout.init = true;
                }
                printAddress(deviceAddress);

                Serial.println();
                // Serial.print(F("[Sens] twout eeprom address: "));
                // printAddressFromEeprom(twoutOffset);
                // Serial.println();

                continue;
            }

            if (tamb_bool)
            {
                for (uint8_t i = 0; i < 8; i++)
                {
                    config.sensors.tamb.addr[i] = deviceAddress[i];
                }
                continue;
            }
            else if (!config.sensors.tamb.init && !twout_bool && !twin_bool)
            {

                Serial.print(F("[Sens] tamb read address: "));

                String temp_str = tamb;
                for (uint8_t i = 0; i < 8; i++)
                {
                    temp_str.concat(i);
                    sensPrefs.putUChar(temp_str.c_str(), deviceAddress[i]);
                    config.sensors.tamb.addr[i] = deviceAddress[i];
                    config.sensors.tamb.init = true;
                }
                printAddress(deviceAddress);

                Serial.println();
                // Serial.print(F("[Sens] tamb eeprom address: "));
                // printAddressFromEeprom(tambOffset);
                // Serial.println();
            }

            if (config.sensors.twin.enabled)
            {

                if (twin_bool)
                {
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        config.sensors.twin.addr[i] = deviceAddress[i];
                    }
                    continue;
                }
                else if (!config.sensors.twin.init && !twout_bool && !tamb_bool)
                {

                    Serial.print(F("[Sens] twin read address: "));

                    String temp_str = twin;
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        temp_str.concat(i);
                        sensPrefs.putUChar(temp_str.c_str(), deviceAddress[i]);
                        config.sensors.twin.addr[i] = deviceAddress[i];
                        config.sensors.twin.init = true;
                    }
                    printAddress(deviceAddress);

                    Serial.println();
                    // Serial.print(F("[Sens] twin eeprom address: "));
                    // printAddressFromEeprom(twinOffset);
                    // Serial.println();
                }
            }
        }
    }
}

float roundTemp(float temp)
{
    int abs = temp;
    float dec = temp - abs;
    if (dec < 0.25)
    {
        temp = abs;
    }
    else if (dec >= 0.25 && dec < 0.75)
    {
        temp = abs + 0.5;
    }
    else
    {
        temp = abs + 1;
    }
    return temp;
}

void showAddressFromPref()
{

    Serial.print(F("[Sens] twout pref address: "));
    printAddressFromPref(twout);
    Serial.println();
    Serial.print(F("[Sens] tamb pref address: "));
    printAddressFromPref(tamb);
    Serial.println();
    Serial.print(F("[Sens] twin pref address: "));
    printAddressFromPref(twin);
    Serial.println();
}

void resetSensorsTempAddr(domopool_Config &config)
{
    sensPrefs.clear();
    config.sensors.twout.init = false;
    config.sensors.twin.init = false;
    config.sensors.tamb.init = false;
}

void initializeDS18B20(domopool_Config &config, DallasTemperature &tempSensors)
{
    tempSensors.begin();
    sensPrefs.begin("sensors");

    Serial.println(F("[Sens] Registering addresses..."));

    registerDevices(config, tempSensors);
    showAddressFromPref();
    tempSensors.setWaitForConversion(config.sensors.wait_for_conversion);
    tempSensors.setResolution(config.sensors.temp_resolution);

    Serial.println(F("[Sens] Setting sensors options..."));
}

bool setADS1115(domopool_Config &config, ADS1115 &ads)
{
    if (ads.isConnected())
    {
        ads.setGain(0);     // 6.144 volt
        ads.setDataRate(7); // fast
        // ads.setMode(0);     // continuous; 1 for single get
        ads.setMode(1); // continuous; 1 for single get
        ads.requestADC(config.sensors.wp.adc_pin);
        config.alarms.ads1115.not_connected = false;
        return false;
    }
    else
    {
        config.alarms.ads1115.not_connected = true;
        return true;
    };
}

void initializeADS115(domopool_Config &config, ADS1115 &ads, int sda, int scl)
{
    if (ads.begin(sda, scl))
    {
        config.alarms.ads1115.not_started = false;
        setADS1115(config, ads);
        Serial.println(F("[Sens] ADS1115 started"));
    }
    else
    {
        config.alarms.ads1115.not_started = true;
        Serial.println(F("[Sens] ADS1115 not started"));
    };
}

// When wp is to far from 0 (when filter pump is stopped), recalibrate threshold
// do not calibrate if threshold is above limits around wp vmin
void autoWPThreshold(domopool_Config &config)
{
    float pct = (config.sensors.wp.vmin * config.sensors.wp.threshold_accuracy) / 100;
    float l_min = config.sensors.wp.vmin - pct;
    float l_max = config.sensors.wp.vmin + pct;
    if (abs(config.metrics.wp) >= config.limits.wp_0_derive)
    {
        if (config.sensors.wp.threshold < l_min || config.sensors.wp.threshold > l_max)
        {
            config.sensors.wp.threshold = config.metrics.wp_volt;
            config.alarms.wp_broken = false;
        }
        else
        {
            config.alarms.wp_broken = true;
            disableWP();
        }
    }
}

float getWPAnalog(domopool_Config &config, ADS1115 &ads)
{
    int16_t raw = 0;
    float val = 0;

    // set ads1115 if not connected before
    if (config.alarms.ads1115.not_connected)
    {
        setADS1115(config, ads);
    }

    // get data from ads1115
    if (ads.isReady())
    {
        raw = ads.getValue();
        ads.requestADC(config.sensors.wp.adc_pin);
        val = ads.toVoltage(raw);
        config.metrics.wp_volt = val;
        config.alarms.ads1115.not_ready = false;
    }
    else
    {
        config.alarms.ads1115.not_ready = true;
    }

    // set analog threshold when pump is off
    if (!config.states.filter_on)
    {
        autoWPThreshold(config);
    }

    return val;
}

void getWP(domopool_Config &config, ADS1115 &ads)
{
    config.metrics.wp = (getWPAnalog(config, ads) - config.sensors.wp.threshold) * 4;
    if (config.metrics.wp >= config.limits.wp_max)
    {
        config.alarms.wp_high = true;
    }
    else
    {
        config.alarms.wp_high = false;
    }

    if (config.metrics.wp <= config.limits.wp_min || config.states.filter_on)
    {
        config.alarms.wp_low = true;
    }
    else
    {
        config.alarms.wp_low = false;
    }
}

void getDS18B20(domopool_Config &config, DallasTemperature &tempSensors)
{
    if (!config.tests.enabled)
    {
        tempSensors.requestTemperatures();
        uint8_t tempAddr[8];

        String temp_str = twout;
        for (uint8_t i = 0; i < 8; i++)
        {
            temp_str.concat(i);
            tempAddr[i] = sensPrefs.getUChar(temp_str.c_str(), 0);
        }
        // printAddressFromPref("twout");

        // Serial.print(F("Sensor 'twout' value: "));
        float twout_val = tempSensors.getTempC(tempAddr);
        Serial.print(F("[Sens] twout temp: "));
        if (twout_val == DEVICE_DISCONNECTED_C)
        {
            Serial.print("could not read twout temperature data");
        }
        else
        {
            Serial.print(twout_val);
        }
        Serial.println();
        float tempMoy = twout_val;

        if (config.sensors.twin.enabled)
        {
            temp_str = twin;
            for (uint8_t i = 0; i < 8; i++)
            {
                temp_str.concat(i);
                tempAddr[i] = sensPrefs.getUChar(temp_str.c_str(), 0);
            }
            float twin_val = tempSensors.getTempC(tempAddr);
            Serial.print(F("[Sens] twin temp: "));
            if (twin_val == DEVICE_DISCONNECTED_C)
            {
                Serial.print("could not read twin temperature data");
            }
            else
            {
                Serial.print(twin_val);
            }
            Serial.println();
            tempMoy = (twout_val + twin_val) / 2;
        }

        config.metrics.twater = tempMoy;

        temp_str = tamb;
        for (uint8_t i = 0; i < 8; i++)
        {
            temp_str.concat(i);
            tempAddr[i] = sensPrefs.getUChar(temp_str.c_str(), 0);
        }
        config.metrics.tamb = tempSensors.getTempC(tempAddr);
        Serial.print(F("[Sens] tamb temp: "));
        if (config.metrics.tamb == DEVICE_DISCONNECTED_C)
        {
            Serial.print("could not read tamb temperature data");
        }
        else
        {
            Serial.print(config.metrics.tamb);
        }
        Serial.println();
    }
    else
    {
        config.metrics.twater = config.tests.twater;
        config.metrics.tamb = config.tests.tamb;
    }
}
