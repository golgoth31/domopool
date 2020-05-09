#include "Asensors.h"

bool checkAddress(DeviceAddress deviceAddress, uint8_t addr[8])
{
    bool ret = true;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (addr[i] != deviceAddress[i])
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
    Serial.println("");
}

void registerDevices(Sensors &config, DallasTemperature tempSensors)
{
    DeviceAddress deviceAddress;

    for (int i = 0; i < 3; i++)
    {
        bool devOk = tempSensors.getAddress(deviceAddress, i);
        if (devOk)
        {
            bool twout = checkAddress(deviceAddress, config.twout.addr);
            bool tamb = checkAddress(deviceAddress, config.tamb.addr);
            bool twin = checkAddress(deviceAddress, config.twin.addr);

            if (!config.twin.enabled)
            {
                twin = false;
            }
            if (!(tamb || twin))
            {
                Serial.println(F("[Sens] twout address: "));
                for (uint8_t i = 0; i < 8; i++)
                {
                    config.twout.addr[i] = deviceAddress[i];
                }
                printAddress(deviceAddress);
            }
            if (!(twout || twin))
            {
                Serial.println(F("[Sens] tamb address: "));
                for (uint8_t i = 0; i < 8; i++)
                {
                    config.tamb.addr[i] = deviceAddress[i];
                }
                printAddress(deviceAddress);
            }
            if (!(twout || tamb))
            {
                Serial.println(F("[Sens] twin address: "));
                for (uint8_t i = 0; i < 8; i++)
                {
                    config.twin.addr[i] = deviceAddress[i];
                }
                printAddress(deviceAddress);
            }
        }
    }
}

float roundTemp(float temp)
{
    float val;
    int abs = temp;
    float dec = temp - abs;
    if (dec < 0.25)
    {
        val = abs;
    }
    else if (dec >= 0.25 && dec < 0.75)
    {
        val = abs;
    }
    else
    {
        val++;
    }
    return val;
}
