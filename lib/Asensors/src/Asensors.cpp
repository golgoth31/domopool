#include "Asensors.h"

Preferences sensPrefs;

String twout = "twout";
String twin = "twin";
String tamb = "tamb";
uint8_t blankFilterStart = 0;
uint8_t blankMaxNumber = 1;

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

    String t_addr;

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
                t_addr = "";
                t_addr = deviceAddress[0];
                for (uint8_t i = 1; i < 8; i++)
                {
                    t_addr.concat(".");
                    t_addr.concat(deviceAddress[i]);
                    // config.sensors.twout.addr[i] = deviceAddress[i];
                }
                strcpy(config.sensors.twout.addr, t_addr.c_str());
                config.sensors.twout.init = true;
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
                // for (uint8_t i = 0; i < 8; i++)
                // {
                //     config.sensors.tamb.addr[i] = deviceAddress[i];
                //     config.sensors.tamb.init = true;
                // }
                t_addr = "";
                t_addr = deviceAddress[0];
                for (uint8_t i = 1; i < 8; i++)
                {
                    t_addr.concat(".");
                    t_addr.concat(deviceAddress[i]);
                }
                strcpy(config.sensors.tamb.addr, t_addr.c_str());
                config.sensors.tamb.init = true;
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
                    // for (uint8_t i = 0; i < 8; i++)
                    // {
                    //     config.sensors.twin.addr[i] = deviceAddress[i];
                    //     config.sensors.twin.init = true;
                    // }
                    t_addr = "";
                    t_addr = deviceAddress[0];
                    for (uint8_t i = 1; i < 8; i++)
                    {
                        t_addr.concat(".");
                        t_addr.concat(deviceAddress[i]);
                    }
                    strcpy(config.sensors.twin.addr, t_addr.c_str());
                    config.sensors.twin.init = true;
                    continue;
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

float roundVal(float val, uint8_t factor)
{
    int abs = val * factor;
    // int dec = (val - abs) * 100;
    // if (dec < 25)
    // {
    //     val = abs;
    // }
    // else if (dec >= 25 && dec < 75)
    // {
    //     val = abs + 0.5;
    // }
    // else
    // {
    //     val = abs + 1;
    // }
    return (float)abs / (float)factor;
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
        ads.setGain(config.sensors.adc_gain);
        ads.setDataRate(config.sensors.adc_datarate);
        ads.setMode(config.sensors.adc_mode);
        config.alarms.ads1115.not_connected = false;
        return false;
    }
    else
    {
        config.alarms.ads1115.not_connected = true;
        return true;
    }
}

void initializeADS115(domopool_Config &config, ADS1115 &ads, int sda, int scl)
{
    if (ads.begin(sda, scl))
    {
        config.alarms.ads1115.not_started = false;
        if (!setADS1115(config, ads))
        {
            ads.requestADC(config.sensors.wp.adc_pin);
            Serial.println(F("[Sens] ADS1115 started"));
        }
    }
    else
    {
        config.alarms.ads1115.not_started = true;
        Serial.println(F("[Sens] ADS1115 not started"));
    };
}

// When wp is to far from 0 (when filter pump is stopped), recalibrate threshold
// do not calibrate if threshold is above limits around wp vmin
uint32_t autoWPThreshold(domopool_Config &config)
{
    uint32_t pct = (config.sensors.wp.vmin * config.sensors.wp.threshold_accuracy) / 100;
    uint32_t l_min = config.sensors.wp.vmin - pct;
    uint32_t l_max = config.sensors.wp.vmin + pct;

    // Check if we are really far from 0.5V
    if (config.metrics.wp_volt > l_min || config.metrics.wp_volt < l_max)
    {
        config.alarms.wp_broken = false;
        config.sensors.wp.threshold = config.metrics.wp_volt;
    }
    else
    {
        config.alarms.wp_broken = true;
        disableWP();
    }

    return 0;
}

void getWPAnalog(domopool_Config &config, ADS1115 &ads)
{
    int16_t raw = 0;
    float val = 0;

    setADS1115(config, ads);

    // get data from ads1115
    if (ads.isReady())
    {
        raw = ads.getValue();
        ads.requestADC(config.sensors.wp.adc_pin);
        val = ads.toVoltage(raw);
        config.alarms.ads1115.not_ready = false;
        config.metrics.wp_volt = val * config.sensors.wp.v_accuracy;
    }
    else
    {
        config.alarms.ads1115.not_ready = true;
    }
}

void getWP(domopool_Config &config, ADS1115 &ads)
{
    getWPAnalog(config, ads);

    config.metrics.wp = (config.metrics.wp_volt - config.sensors.wp.threshold) * 4;

    // autocalibrate if we have derived from 0 Bar
    if (!config.states.filter_on && config.sensors.wp.auto_cal && config.metrics.wp >= config.limits.wp_0_derive)
    {
        config.metrics.wp = autoWPThreshold(config);
    }

    // Check if we have a high water pressure value
    if (config.metrics.wp >= config.limits.wp_max)
    {
        config.alarms.wp_high = true;
        config.alarms.wp_value = config.metrics.wp;
    }
    // else
    // {
    //     config.alarms.wp_high = false;
    // }

    if (config.states.filter_on)
    {
        if (config.metrics.wp <= config.limits.wp_min)
        {
            if (blankFilterStart == blankMaxNumber)
            {
                config.alarms.wp_low = true;
                config.alarms.wp_value = config.metrics.wp;
            }
            else
            {
                blankFilterStart++;
            }
        }
    }
    else
    {
        blankFilterStart = 0;
    }

    // if (config.tests.enabled)
    // {
    //     config.metrics.wp = config.tests.pressure;
    // }
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
                Serial.print("could not read twin");
            }
            else
            {
                Serial.print(twin_val);
            }
            Serial.println();
            tempMoy = (twout_val + twin_val) / 2;
        }

        config.metrics.twater = roundVal(tempMoy, config.sensors.precision_factor);

        temp_str = tamb;
        for (uint8_t i = 0; i < 8; i++)
        {
            temp_str.concat(i);
            tempAddr[i] = sensPrefs.getUChar(temp_str.c_str(), 0);
        }
        config.metrics.tamb = roundVal(tempSensors.getTempC(tempAddr), config.sensors.precision_factor);
        Serial.print(F("[Sens] tamb temp: "));
        if (config.metrics.tamb == DEVICE_DISCONNECTED_C)
        {
            Serial.print("could not read tamb");
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

    if (config.metrics.twater < config.limits.tw_min)
    {
        config.alarms.tw_frost = true;
    }
    else
    {
        config.alarms.tw_frost = false;
    }

    if (config.metrics.twater >= config.limits.tw_max)
    {
        config.alarms.tw_high = true;
    }
    else
    {
        config.alarms.tw_high = false;
    }

    if (config.metrics.tamb <= config.limits.tamb_min)
    {
        config.alarms.tamb_frost = true;
    }
    else
    {
        config.alarms.tamb_frost = false;
    }
}
