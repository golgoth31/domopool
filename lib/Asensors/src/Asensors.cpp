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

void registerDevices(Sensors &config, DallasTemperature &tempSensors)
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
            else if (!config.twout.init && !tamb && !twin)
            {

                Serial.print(F("[Sens] twout read address: "));

                for (uint8_t i = 0; i < 8; i++)
                {
                    sensPrefs.putUChar("twout" + i, deviceAddress[i]);
                    config.twout.addr[i] = deviceAddress[i];
                    config.twout.init = true;
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
            else if (!config.tamb.init && !twout && !twin)
            {

                Serial.print(F("[Sens] tamb read address: "));

                for (uint8_t i = 0; i < 8; i++)
                {
                    sensPrefs.putUChar("tamb" + i, deviceAddress[i]);
                    config.tamb.addr[i] = deviceAddress[i];
                    config.tamb.init = true;
                }
                printAddress(deviceAddress);

                Serial.println();
                // Serial.print(F("[Sens] tamb eeprom address: "));
                // printAddressFromEeprom(tambOffset);
                // Serial.println();
            }

            if (config.twin.enabled)
            {

                if (twin)
                {
                    continue;
                }
                else if (!config.twin.init && !twout && !tamb)
                {

                    Serial.print(F("[Sens] twin read address: "));

                    for (uint8_t i = 0; i < 8; i++)
                    {
                        sensPrefs.putUChar("twin" + i, deviceAddress[i]);
                        config.twin.addr[i] = deviceAddress[i];
                        config.twin.init = true;
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

void initializeDS18B20(Sensors &config, DallasTemperature &tempSensors)
{
    tempSensors.begin();
    sensPrefs.begin("sensors");

    Serial.println(F("[Sens] Registering addresses..."));

    registerDevices(config, tempSensors);
    showAddressFromPref();

    Serial.println(F("[Sens] Setting sensors options..."));

    tempSensors.setWaitForConversion(config.waitForConversion);
    tempSensors.setResolution(config.tempResolution);
}

void getDS18B20(Config &config, DallasTemperature &tempSensors)
{
    if (!config.tests.enabled)
    {
        tempSensors.requestTemperatures();

        // Serial.print(F("Sensor 'twout' value: "));
        float twout = tempSensors.getTempC(config.sensors.twout.addr);
        // Serial.println(config.sensors.twout.val);
        float tempMoy = twout;
        if (config.sensors.twin.enabled)
        {
            // Serial.print(F("Sensor 'twin' value: "));
            float twin = tempSensors.getTempC(config.sensors.twin.addr);
            // Serial.println(config.sensors.twin.val);
            tempMoy = (twout + twin) / 2;
        }
        config.metrics.curTempWater = roundTemp(tempMoy);
        config.metrics.curTempAmbiant = tempSensors.getTempC(config.sensors.tamb.addr);
    }
    else
    {
        config.metrics.curTempWater = config.tests.twater;
        config.metrics.curTempAmbiant = config.tests.tamb;
    }
}
