#include "Aconfig.h"

// Loads the configuration from a file
void loadConfiguration(const char *filename, Config &config)
{
    // Open file for reading
    File file = openFile(filename);

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<ConfigDocSize> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {

        Serial.println(F("[Conf] Failed to read file, using default configuration"));

        doc["network"]["dhcp"] = true;
        doc["network"]["allowPost"] = false;
        for (int i = 0; i < 8; i++)
        {
            doc["sensors"]["twin"]["addr"][i] = 0;
            doc["sensors"]["twout"]["addr"][i] = 0;
            doc["sensors"]["tamb"]["addr"][i] = 0;
        }
        doc["sensors"]["twin"]["enabled"] = false;
        doc["sensors"]["twout"]["init"] = false;
        doc["sensors"]["tamb"]["init"] = false;
        doc["sensors"]["twin"]["init"] = false;
        doc["sensors"]["waitForConvertion"] = true;
        doc["sensors"]["tempResolution"] = 12;
        doc["sensors"]["ph"]["enabled"] = false;
        doc["sensors"]["ph"]["threshold"] = 7.4;
        doc["global"]["lcdBacklightDuration"] = 30000; // 30s
        doc["time"]["initialized"] = false;
        doc["time"]["dayLight"] = 3600;
        doc["time"]["ntpServer"] = "europe.pool.ntp.org";
        doc["time"]["timeZone"] = 3600;
        doc["pump"]["forceFilter"] = false;
        doc["pump"]["forcePH"] = false;
        doc["pump"]["forceCH"] = false;
    }

    // Copy values from the JsonDocument to the Config
    convert2config(doc, config);

    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
}

// Saves the configuration to a file
bool saveConfiguration(const char *filename, Config &config)
{
    File file = saveFile(filename);
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<ConfigDocSize> doc;
    config2doc(config, doc);

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0)
    {

        Serial.println(F("Failed to write to file"));

        return false;
    }

    // Close the file
    file.close();
    return true;
}

// Saves the configuration to a file
bool saveJson(String &data, Config &config, const char *filename)
{
    StaticJsonDocument<ConfigDocSize> doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error)
    {
        return false;
    }
    convert2config(doc, config);
    bool saved = saveConfiguration(filename, config);
    if (!saved)
    {
        return false;
    }
    return true;
}

// Saves the configuration to a file
// StaticJsonDocument<ConfigDocSize> convert2doc(Config &config)
void config2doc(Config &config, JsonDocument &doc)
{
    doc["global"]["lcdBacklightDuration"] = config.global.lcdBacklightDuration;
    doc["time"]["initialized"] = config.time.initialized;
    doc["time"]["dayLight"] = config.time.dayLight;
    doc["time"]["ntpServer"] = config.time.ntpServer;
    doc["time"]["timeZone"] = config.time.timeZone;
    doc["network"]["dhcp"] = config.network.dhcp;
    doc["network"]["ip"] = config.network.ip;
    doc["network"]["gateway"] = config.network.gateway;
    doc["network"]["netmask"] = config.network.netmask;
    doc["network"]["dns"] = config.network.dns;
    doc["sensors"]["waitForConvertion"] = config.sensors.waitForConversion;
    doc["sensors"]["tempResolution"] = config.sensors.tempResolution;
    doc["sensors"]["twin"]["enabled"] = config.sensors.twin.enabled;
    doc["sensors"]["twin"]["init"] = config.sensors.twin.init;
    for (int i = 0; i < 8; i++)
    {
        doc["sensors"]["twin"]["addr"][i] = config.sensors.twin.addr[i];
    }

    doc["sensors"]["twout"]["enabled"] = true;
    doc["sensors"]["twout"]["init"] = config.sensors.twout.init;
    for (int i = 0; i < 8; i++)
    {
        doc["sensors"]["twout"]["addr"][i] = config.sensors.twout.addr[i];
    }
    doc["sensors"]["tamb"]["enabled"] = true;
    doc["sensors"]["tamb"]["init"] = config.sensors.tamb.init;
    for (int i = 0; i < 8; i++)
    {
        doc["sensors"]["tamb"]["addr"][i] = config.sensors.tamb.addr[i];
    }
    doc["sensors"]["ph"]["enabled"] = config.sensors.ph.enabled;
    doc["sensors"]["ph"]["threshold"] = config.sensors.ph.threshold;
    doc["pump"]["forceFilter"] = config.pump.forceFilter;
    doc["pump"]["forcePH"] = config.pump.forcePH;
    doc["pump"]["forceCH"] = config.pump.forceCH;
}

void metrics2doc(Config &config, JsonDocument &doc)
{
    doc["metrics"]["chDuration"] = config.metrics.chDuration;
    doc["metrics"]["chOn"] = config.metrics.chOn;
    doc["metrics"]["curCh"] = config.metrics.curCh;
    doc["metrics"]["curPh"] = config.metrics.curPh;
    doc["metrics"]["curTempAmbiant"] = config.metrics.curTempAmbiant;
    doc["metrics"]["curTempWater"] = config.metrics.curTempWater;
    doc["metrics"]["filterOn"] = config.metrics.filterOn;
    doc["metrics"]["hour"] = config.metrics.hour;
    doc["metrics"]["phOn"] = config.metrics.phOn;
    doc["metrics"]["savedTempWater"] = config.metrics.savedTempWater;
    doc["metrics"]["startup"] = config.metrics.startup;
}

void convert2config(JsonDocument &doc, Config &config)
{
    config.global.lcdBacklightDuration = doc["global"]["lcdBacklightDuration"];
    config.time.initialized = doc["time"]["initialized"];
    config.time.dayLight = doc["time"]["dayLight"];
    config.time.ntpServer = doc["time"]["ntpServer"];
    config.time.timeZone = doc["time"]["timeZone"];
    config.metrics.alarms.storage = doc["data"]["alarms"]["storage"];
    config.network.dhcp = doc["network"]["dhcp"];
    config.network.allowPost = doc["network"]["allowPost"];
    config.network.ip = doc["network"]["ip"];
    config.network.gateway = doc["network"]["gateway"];
    config.network.netmask = doc["network"]["netmask"];
    config.network.dns = doc["network"]["dns"];
    config.sensors.ph.enabled = doc["sensors"]["ph"]["enabled"];
    config.sensors.ph.threshold = doc["sensors"]["ph"]["threshold"];
    config.sensors.waitForConversion = doc["sensors"]["waitForConvertion"];
    config.sensors.tempResolution = doc["sensors"]["tempResolution"];
    config.sensors.twin.enabled = doc["sensors"]["twin"]["enabled"];
    config.sensors.twin.init = doc["sensors"]["twin"]["init"];
    for (int i = 0; i < 8; i++)
    {
        config.sensors.twin.addr[i] = doc["sensors"]["twin"]["addr"][i];
    }
    config.sensors.twout.enabled = true;
    config.sensors.twout.init = doc["sensors"]["twout"]["init"];
    for (int i = 0; i < 8; i++)
    {
        config.sensors.twout.addr[i] = doc["sensors"]["twout"]["addr"][i];
    }
    config.sensors.tamb.enabled = true;
    config.sensors.tamb.init = doc["sensors"]["tamb"]["init"];
    for (int i = 0; i < 8; i++)
    {
        config.sensors.tamb.addr[i] = doc["sensors"]["tamb"]["addr"][i];
    }
    config.pump.forceFilter = doc["pump"]["forceFilter"];
    config.pump.forcePH = doc["pump"]["forcePH"];
    config.pump.forceCH = doc["pump"]["forceCH"];
}

void initConfigData(Config &config)
{
    config.metrics.startup = true;
    config.metrics.filterOn = false;
    config.metrics.phOn = false;
}
