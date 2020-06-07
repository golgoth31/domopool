#include "Asensors.h"

#include <EEPROM.h>

const int twoutOffset = 0;
const int twinOffset = 16;
const int tambOffset = 8;

bool checkAddress(DeviceAddress deviceAddress, int offset)
{
    bool ret = true;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (EEPROM.read(i + offset) != deviceAddress[i])
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

void printAddressFromEeprom(int offset)
{

    for (uint8_t i = 0; i < 8; i++)
    {
        Serial.print(F("0x"));
        if (EEPROM.read(i + offset) < 0x10)
        {
            Serial.print(F("0"));
        }
        Serial.print(EEPROM.read(i + offset), HEX);
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
            twout = checkAddress(deviceAddress, twoutOffset);
            tamb = checkAddress(deviceAddress, tambOffset);
            twin = checkAddress(deviceAddress, twinOffset);

            if (twout)
            {
                continue;
            }
            else if (!config.twout.init && !tamb && !twin)
            {

                Serial.print(F("[Sens] twout read address: "));

                for (uint8_t i = 0; i < 8; i++)
                {
                    EEPROM.write(i + twoutOffset, deviceAddress[i]);
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
                    EEPROM.write(i + tambOffset, deviceAddress[i]);
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
                        EEPROM.write(i + twinOffset, deviceAddress[i]);
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

void showAddressFromEeprom()
{

    Serial.print(F("[Sens] twout eeprom address: "));
    printAddressFromEeprom(twoutOffset);
    Serial.println();
    Serial.print(F("[Sens] tamb eeprom address: "));
    printAddressFromEeprom(tambOffset);
    Serial.println();
    Serial.print(F("[Sens] twin eeprom address: "));
    printAddressFromEeprom(twinOffset);
    Serial.println();
}

void resetEepromSensorsTemp()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        EEPROM.write(i + twoutOffset, 0);
        EEPROM.write(i + tambOffset, 0);
        EEPROM.write(i + twinOffset, 0);
    }
}
