#include "Aconfig.h"

// Loads the configuration from a file
void loadConfiguration(const char *filename, Config &config)
{
    // Open file for reading
    File file = openFile(filename);
    StaticJsonDocument<ConfigDocSize> doc;
    JsonObject root;
    Serial.println(file.readString());
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.println(F("[Conf] Failed to read file, using default configuration"));
        Serial.println(error.c_str());
        root = doc.to<JsonObject>();
        root["network"]["dhcp"] = true;
        root["network"]["allowPost"] = false;
        for (int i = 0; i < 8; i++)
        {
            root["sensors"]["twin"]["addr"][i] = 0;
            root["sensors"]["twout"]["addr"][i] = 0;
            root["sensors"]["tamb"]["addr"][i] = 0;
        }
        root["sensors"]["twin"]["enabled"] = false;
        root["sensors"]["twout"]["init"] = false;
        root["sensors"]["tamb"]["init"] = false;
        root["sensors"]["twin"]["init"] = false;
        root["sensors"]["waitForConvertion"] = true;
        root["sensors"]["tempResolution"] = 12;
        root["sensors"]["ph"]["enabled"] = false;
        root["sensors"]["ph"]["threshold"] = 7.4;
        root["global"]["lcdBacklightDuration"] = 30000; // 30s
        root["time"]["initialized"] = false;
        root["time"]["dayLight"] = 3600;
        root["time"]["ntpServer"] = "europe.pool.ntp.org";
        root["time"]["timeZone"] = 3600;
        root["pump"]["forceFilter"] = false;
        root["pump"]["forcePH"] = false;
        root["pump"]["forceCH"] = false;
    }
    else
    {
        root = doc.as<JsonObject>();
    }

    // Copy values from the JsonDocument to the Config
    object2config(root, config);

    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
}

// Saves the configuration to a file
bool saveConfiguration(const char *filename, Config &config)
{
    File file = saveFile(filename);
    StaticJsonDocument<ConfigDocSize> doc;
    config2doc(config, doc);

    // Serialize JSON to file
    // serializeJson(doc, Serial);
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
bool saveJson(JsonObject &json, Config &config, const char *filename)
{
    object2config(json, config);
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
    JsonObject jsonObj = doc.to<JsonObject>();
    jsonObj["global"]["lcdBacklightDuration"] = config.global.lcdBacklightDuration;
    jsonObj["time"]["initialized"] = config.time.initialized;
    jsonObj["time"]["dayLight"] = config.time.dayLight;
    jsonObj["time"]["ntpServer"] = config.time.ntpServer;
    jsonObj["time"]["timeZone"] = config.time.timeZone;
    jsonObj["network"]["dhcp"] = config.network.dhcp;
    jsonObj["network"]["ip"] = config.network.ip;
    jsonObj["network"]["gateway"] = config.network.gateway;
    jsonObj["network"]["netmask"] = config.network.netmask;
    jsonObj["network"]["dns"] = config.network.dns;
    jsonObj["network"]["mqtt"]["enabled"] = config.network.mqtt.enabled;
    jsonObj["sensors"]["waitForConvertion"] = config.sensors.waitForConversion;
    jsonObj["sensors"]["tempResolution"] = config.sensors.tempResolution;
    jsonObj["sensors"]["twin"]["enabled"] = config.sensors.twin.enabled;
    jsonObj["sensors"]["twin"]["init"] = config.sensors.twin.init;
    for (int i = 0; i < 8; i++)
    {
        jsonObj["sensors"]["twin"]["addr"][i] = config.sensors.twin.addr[i];
    }

    jsonObj["sensors"]["twout"]["enabled"] = true;
    jsonObj["sensors"]["twout"]["init"] = config.sensors.twout.init;
    for (int i = 0; i < 8; i++)
    {
        jsonObj["sensors"]["twout"]["addr"][i] = config.sensors.twout.addr[i];
    }
    jsonObj["sensors"]["tamb"]["enabled"] = true;
    jsonObj["sensors"]["tamb"]["init"] = config.sensors.tamb.init;
    for (int i = 0; i < 8; i++)
    {
        jsonObj["sensors"]["tamb"]["addr"][i] = config.sensors.tamb.addr[i];
    }
    jsonObj["sensors"]["ph"]["enabled"] = config.sensors.ph.enabled;
    jsonObj["sensors"]["ph"]["threshold"] = config.sensors.ph.threshold;
    jsonObj["pump"]["forceFilter"] = config.pump.forceFilter;
    jsonObj["pump"]["forcePH"] = config.pump.forcePH;
    jsonObj["pump"]["forceCH"] = config.pump.forceCH;
}

void metrics2doc(Config &config, JsonDocument &doc)
{
    JsonObject jsonObj = doc.as<JsonObject>();
    jsonObj["metrics"]["chDuration"] = config.metrics.chDuration;
    jsonObj["metrics"]["chOn"] = config.metrics.chOn;
    jsonObj["metrics"]["curCh"] = config.metrics.curCh;
    jsonObj["metrics"]["curPh"] = config.metrics.curPh;
    jsonObj["metrics"]["curTempAmbiant"] = config.metrics.curTempAmbiant;
    jsonObj["metrics"]["curTempWater"] = config.metrics.curTempWater;
    jsonObj["metrics"]["curWaterPressure"] = config.metrics.curWaterPressure;
    jsonObj["metrics"]["filterOn"] = config.metrics.filterOn;
    jsonObj["metrics"]["hour"] = config.metrics.hour;
    jsonObj["metrics"]["phOn"] = config.metrics.phOn;
    jsonObj["metrics"]["savedTempWater"] = config.metrics.savedTempWater;
    jsonObj["metrics"]["startup"] = config.metrics.startup;
}

void convert2config(JsonDocument &doc, Config &config)
{
    JsonObject jsonObj = doc.as<JsonObject>();
    object2config(jsonObj, config);
    // config.global.lcdBacklightDuration = doc["global"]["lcdBacklightDuration"];
    // config.time.initialized = doc["time"]["initialized"];
    // config.time.dayLight = doc["time"]["dayLight"];
    // config.time.ntpServer = doc["time"]["ntpServer"];
    // config.time.timeZone = doc["time"]["timeZone"];
    // config.metrics.alarms.storage = doc["data"]["alarms"]["storage"];
    // config.network.dhcp = doc["network"]["dhcp"];
    // config.network.allowPost = doc["network"]["allowPost"];
    // config.network.ip = doc["network"]["ip"];
    // config.network.gateway = doc["network"]["gateway"];
    // config.network.netmask = doc["network"]["netmask"];
    // config.network.dns = doc["network"]["dns"];
    // config.sensors.ph.enabled = doc["sensors"]["ph"]["enabled"];
    // config.sensors.ph.threshold = doc["sensors"]["ph"]["threshold"];
    // config.sensors.waitForConversion = doc["sensors"]["waitForConvertion"];
    // config.sensors.tempResolution = doc["sensors"]["tempResolution"];
    // config.sensors.twin.enabled = doc["sensors"]["twin"]["enabled"];
    // config.sensors.twin.init = doc["sensors"]["twin"]["init"];
    // for (int i = 0; i < 8; i++)
    // {
    //     config.sensors.twin.addr[i] = doc["sensors"]["twin"]["addr"][i];
    // }
    // config.sensors.twout.enabled = true;
    // config.sensors.twout.init = doc["sensors"]["twout"]["init"];
    // for (int i = 0; i < 8; i++)
    // {
    //     config.sensors.twout.addr[i] = doc["sensors"]["twout"]["addr"][i];
    // }
    // config.sensors.tamb.enabled = true;
    // config.sensors.tamb.init = doc["sensors"]["tamb"]["init"];
    // for (int i = 0; i < 8; i++)
    // {
    //     config.sensors.tamb.addr[i] = doc["sensors"]["tamb"]["addr"][i];
    // }
    // config.pump.forceFilter = doc["pump"]["forceFilter"];
    // config.pump.forcePH = doc["pump"]["forcePH"];
    // config.pump.forceCH = doc["pump"]["forceCH"];
}
void object2config(JsonObject doc, Config &config)
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
    config.network.mqtt.enabled = doc["network"]["mqtt"]["enabled"];
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
