#include "Asensors.h"

Preferences sensPrefs;

bool checkAddress(DeviceAddress deviceAddress, const char *temp)
{
    bool ret = true;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (sensPrefs.getUChar(temp + i, 0) != deviceAddress[i])
        {
            ret = false;
        }
    }
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

void printAddressFromPref(const char *temp)
{

    for (uint8_t i = 0; i < 8; i++)
    {
        Serial.print(F("0x"));
        if (sensPrefs.getUChar(temp + i, 0) < 0x10)
        {
            Serial.print(F("0"));
        }
        Serial.print(sensPrefs.getUChar(temp + i, 0), HEX);
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

    bool twout = false;
    bool tamb = false;
    bool twin = false;

    for (int j = 0; j < num; j++)
    {
        bool devOk = tempSensors.getAddress(deviceAddress, j);
        if (devOk)
        {
            twout = checkAddress(deviceAddress, "twout");
            tamb = checkAddress(deviceAddress, "tamb");
            twin = checkAddress(deviceAddress, "twin");

            if (twout)
            {
                continue;
            }
            else if (!config.sensors.twout.init && !tamb && !twin)
            {

                Serial.print(F("[Sens] twout read address: "));

                for (uint8_t i = 0; i < 8; i++)
                {
                    sensPrefs.putUChar("twout" + i, deviceAddress[i]);
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

            if (tamb)
            {
                continue;
            }
            else if (!config.sensors.tamb.init && !twout && !twin)
            {

                Serial.print(F("[Sens] tamb read address: "));

                for (uint8_t i = 0; i < 8; i++)
                {
                    sensPrefs.putUChar("tamb" + i, deviceAddress[i]);
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

                if (twin)
                {
                    continue;
                }
                else if (!config.sensors.twin.init && !twout && !tamb)
                {

                    Serial.print(F("[Sens] twin read address: "));

                    for (uint8_t i = 0; i < 8; i++)
                    {
                        sensPrefs.putUChar("twin" + i, deviceAddress[i]);
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
    Serial.println(sensPrefs.getString("twout", "not saved"));
    Serial.print(F("[Sens] tamb pref address: "));
    Serial.println(sensPrefs.getString("tamb", "not saved"));
    Serial.print(F("[Sens] twin pref address: "));
    Serial.println(sensPrefs.getString("twin", "not saved"));
}

void resetSensorsTempAddr()
{
    sensPrefs.clear();
}

void initializeDS18B20(domopool_Config &config, DallasTemperature &tempSensors)
{
    tempSensors.begin();
    sensPrefs.begin("sensors");

    Serial.println(F("[Sens] Registering addresses..."));

    registerDevices(config, tempSensors);
    showAddressFromPref();

    Serial.println(F("[Sens] Setting sensors options..."));

    tempSensors.setWaitForConversion(config.sensors.wait_for_conversion);
    tempSensors.setResolution(config.sensors.temp_resolution);
}

void initializeADS115(domopool_Config &config, Adafruit_ADS1115 &ads)
{
    ads.begin();
    Serial.println(F("[Sens] ADS1115 started"));
}

void getWP(domopool_Config &config, Adafruit_ADS1115 &ads)
{
    if (config.sensors.water_pressure.enabled)
    {
        config.metrics.water_pressure = (getWPAnalog(config.sensors.water_pressure.adc_pin, ads) - config.sensors.water_pressure.threshold) * 4;
    }
}

float getWPAnalog(int pin, Adafruit_ADS1115 &ads)
{
    return (ads.readADC_SingleEnded(pin) * 0.1875) / 1000;
}

void getDS18B20(domopool_Config &config, DallasTemperature &tempSensors)
{
    if (!config.tests.enabled)
    {
        tempSensors.requestTemperatures();

        uint8_t tempAddr[8];
        for (uint8_t i = 0; i < 8; i++)
        {
            tempAddr[i] = sensPrefs.getUChar("tamb" + i, 0);
        }

        // Serial.print(F("Sensor 'twout' value: "));
        float twout = tempSensors.getTempC(tempAddr);
        // Serial.println(config.sensors.twout.val);
        float tempMoy = twout;
        if (config.sensors.twin.enabled)
        {
            // Serial.print(F("Sensor 'twin' value: "));
            for (uint8_t i = 0; i < 8; i++)
            {
                tempAddr[i] = sensPrefs.getUChar("tamb" + i, 0);
            }
            float twin = tempSensors.getTempC(tempAddr);
            // Serial.println(config.sensors.twin.val);
            tempMoy = (twout + twin) / 2;
        }
        config.metrics.twater = roundTemp(tempMoy);

        for (uint8_t i = 0; i < 8; i++)
        {
            tempAddr[i] = sensPrefs.getUChar("tamb" + i, 0);
        }
        config.metrics.tamb = tempSensors.getTempC(tempAddr);
    }
    else
    {
        config.metrics.twater = config.tests.twater;
        config.metrics.tamb = config.tests.tamb;
    }
}
