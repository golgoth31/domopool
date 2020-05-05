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
        Serial.print("0x");
        if (deviceAddress[i] < 0x10)
        {
            Serial.print("0");
        }
        Serial.print(deviceAddress[i], HEX);
        if (i < 7)
        {
            Serial.print(", ");
        }
    }
    Serial.println("");
}

void registerDevices(Sensors &config, DallasTemperature tempSensors)
{
    DeviceAddress deviceAddress;
    bool devOk;

    for (int i = 0; i < 2; i++)
    {
        Serial.print("[Sens] Sensor ");
        Serial.print(i + 1);
        Serial.print(" : ");
        devOk = tempSensors.getAddress(deviceAddress, i);
        printAddress(deviceAddress);
        switch (i)
        {
        case 0:
            // Connect tout device and let the system register it
            // Do we have registerd this sensor as tout
            if (!checkAddress(deviceAddress, config.tout.addr))
            {
                // Do we have registerd this sensor as tin, in case we replace a deffective device
                if (!checkAddress(deviceAddress, config.tin.addr))
                {
                    // This device hasn't been registered, must be connected to tout
                    Serial.println(F("[Sens] Updating tout configuration"));
                    for (uint8_t i = 0; i < 8; i++)
                    {
                        config.tout.addr[i] = deviceAddress[i];
                    }
                }
            }
            break;
        case 1:
            // we have a second device connected, set it as tin
            if (devOk)
            {
                if (!checkAddress(deviceAddress, config.tin.addr))
                {
                    if (!checkAddress(deviceAddress, config.tout.addr))
                    {
                        Serial.println(F("[Sens] Updating tin configuration"));
                        for (uint8_t i = 0; i < 8; i++)
                        {
                            config.tin.addr[i] = deviceAddress[i];
                        }
                        config.tin.enabled = true;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}
