#include "Aconfig.h"

// Loads the configuration from a file
void loadConfiguration(const char *filename, Aconfig &config)
{
    // Open file for reading
    File file = SD.open(filename);

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<AconfigDocSize> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.println(F("[Conf] Failed to read file, using default configuration"));
        doc["network"]["dhcp"] = true;
        for (int i = 0; i < 8; i++)
        {
            doc["sensors"]["tin"]["addr"][i] = 0;
            doc["sensors"]["tout"]["addr"][i] = 0;
        }
        doc["sensors"]["tin"]["enabled"] = false;
        doc["sensors"]["tdht"]["enabled"] = true;
        doc["sensors"]["waitForConvertion"] = false;
        doc["sensors"]["tempResolution"] = 12;
        doc["sensors"]["ph"]["enabled"] = false;
        doc["sensors"]["ph"]["threshold"] = 7.4;
        doc["global"]["lcdBacklightDuration"] = 30000; // 30s
        doc["time"]["initialized"] = false;
        doc["time"]["dayLight"] = true;
        doc["time"]["ntpServer"] = "europe.pool.ntp.org";
        doc["time"]["timeZone"] = 2;
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
bool saveConfiguration(const char *filename, Aconfig &config)
{
    // Delete existing file, otherwise the configuration is appended to the file
    char *file2remove;
    file2remove = const_cast<char *>(filename);
    SD.remove(file2remove);

    Serial.print(F("[Conf] Opening "));
    Serial.print(filename);
    Serial.println(F("..."));
    // Open file for writing
    File file = SD.open(filename, FILE_WRITE);
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        return false;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<AconfigDocSize> doc;
    convert2doc(config, doc);

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
bool saveJson(String &data, Aconfig &config, const char *filename)
{
    StaticJsonDocument<AconfigDocSize> doc;
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
// StaticJsonDocument<AconfigDocSize> convert2doc(Aconfig &config)
void convert2doc(Aconfig &config, JsonDocument &doc)
{
    doc["global"]["lcdBacklightDuration"] = config.global.lcdBacklightDuration;
    doc["time"]["initialized"] = config.time.initialized;
    doc["time"]["dayLight"] = config.time.dayLight;
    doc["time"]["ntpServer"] = config.time.ntpServer;
    doc["time"]["timeZone"] = config.time.timeZone;
    doc["network"]["dhcp"] = config.netConfig.dhcp;
    doc["network"]["ip"] = config.netConfig.ip;
    doc["network"]["gateway"] = config.netConfig.gateway;
    doc["network"]["netmask"] = config.netConfig.netmask;
    doc["network"]["dns"] = config.netConfig.dns;
    doc["sensors"]["waitForConvertion"] = config.sensConfig.waitForConversion;
    doc["sensors"]["tempResolution"] = config.sensConfig.tempResolution;
    doc["sensors"]["tin"]["enabled"] = config.sensConfig.tin.enabled;
    doc["sensors"]["tin"]["val"] = config.sensConfig.tin.val;
    for (int i = 0; i < 8; i++)
    {
        doc["sensors"]["tin"]["addr"][i] = config.sensConfig.tin.addr[i];
    }

    doc["sensors"]["tout"]["enabled"] = true;
    doc["sensors"]["tout"]["val"] = config.sensConfig.tout.val;
    for (int i = 0; i < 8; i++)
    {
        doc["sensors"]["tout"]["addr"][i] = config.sensConfig.tout.addr[i];
    }
    doc["sensors"]["tdht"]["enabled"] = config.sensConfig.tdht.enabled;
    doc["sensors"]["tdht"]["val"] = config.sensConfig.tdht.val;
    doc["sensors"]["ph"]["enabled"] = config.sensConfig.ph.enabled;
    doc["sensors"]["ph"]["threshold"] = config.sensConfig.ph.threshold;
    doc["sensors"]["ph"]["val"] = config.sensConfig.ph.val;
    doc["pump"]["forceFilter"] = config.pump.forceFilter;
    doc["pump"]["forcePH"] = config.pump.forcePH;
    doc["pump"]["forceCH"] = config.pump.forceCH;
}

void convert2config(JsonDocument &doc, Aconfig &config)
{
    config.global.lcdBacklightDuration = doc["global"]["lcdBacklightDuration"];
    config.time.initialized = doc["time"]["initialized"];
    config.time.dayLight = doc["time"]["dayLight"];
    config.time.ntpServer = doc["time"]["ntpServer"];
    config.time.timeZone = doc["time"]["timeZone"];
    config.netConfig.dhcp = doc["network"]["dhcp"];
    config.netConfig.ip = doc["network"]["ip"];
    config.netConfig.gateway = doc["network"]["gateway"];
    config.netConfig.netmask = doc["network"]["netmask"];
    config.netConfig.dns = doc["network"]["dns"];
    config.sensConfig.ph.enabled = doc["sensors"]["ph"]["enabled"];
    config.sensConfig.ph.threshold = doc["sensors"]["ph"]["threshold"];
    config.sensConfig.waitForConversion = doc["sensors"]["waitForConvertion"];
    config.sensConfig.tempResolution = doc["sensors"]["tempResolution"];
    config.sensConfig.tin.enabled = doc["sensors"]["tin"]["enabled"];
    for (int i = 0; i < 8; i++)
    {
        config.sensConfig.tin.addr[i] = doc["sensors"]["tin"]["addr"][i];
    }
    config.sensConfig.tout.enabled = true;
    for (int i = 0; i < 8; i++)
    {
        config.sensConfig.tout.addr[i] = doc["sensors"]["tout"]["addr"][i];
    }
    config.sensConfig.tdht.enabled = doc["sensors"]["tdht"]["enabled"];
    config.pump.forceFilter = doc["pump"]["forceFilter"];
    config.pump.forcePH = doc["pump"]["forcePH"];
    config.pump.forceCH = doc["pump"]["forceCH"];
}
