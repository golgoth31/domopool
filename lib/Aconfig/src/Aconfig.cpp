#include "Aconfig.h"

const int ConfigDocSize = 1024;
const char *networkFile = "/network.jsn";
Preferences prefs;

void pref2config(Config &config)
{
    String defaultNtpServer = "europe.pool.ntp.org";
    String defaultMQTTServer = "192.168.10.194";
    double_t defaultAckTone = 4000;
    float_t defaultPhThreshold = 7.4;

    //default not working
    config.global.ackTone = prefs.getDouble("ack_tone", defaultAckTone);
    config.time.ntpServer = prefs.getString("ntp_server", defaultNtpServer);
    config.network.mqtt.server = prefs.getString("mqtt_server", defaultMQTTServer);
    config.sensors.ph.threshold = prefs.getFloat("ph_threshold", defaultPhThreshold);

    //no bug
    config.global.lcdBacklightDuration = prefs.getShort("BacklightTime", 30000);
    config.global.ackDuration = prefs.getInt("ackDuration", 100);
    config.global.serialOut = prefs.getBool("serialOut", true);
    config.global.displayStartup = prefs.getBool("displayStartup", false);
    config.time.initialized = prefs.getBool("time_init", false);
    config.time.dayLight = prefs.getShort("dayLight", 3600);
    config.time.timeZone = prefs.getShort("timeZone", 3600);
    config.network.dhcp = prefs.getBool("dhcp", true);
    config.network.allowPost = prefs.getBool("allowPost", true);
    config.network.mqtt.enabled = prefs.getBool("mqtt_enabled", false);
    config.sensors.ph.enabled = prefs.getBool("ph_enabled", false);
    config.sensors.waitForConversion = prefs.getBool("waitConvertion", false);
    config.sensors.tempResolution = prefs.getShort("tempResolution", 12);
    config.sensors.twin.enabled = prefs.getBool("twin_enabled", false);
    config.sensors.twin.init = prefs.getBool("twin_init", false);
    config.sensors.twout.enabled = true;
    config.sensors.twout.init = prefs.getBool("twout_init", false);
    config.sensors.tamb.enabled = true;
    config.sensors.tamb.init = prefs.getBool("tamb_init", false);
    config.pump.forceFilter = prefs.getBool("forceFilter", false);
    config.pump.forcePH = prefs.getBool("forcePH", false);
    config.pump.forceCH = prefs.getBool("forceCH", false);
    config.pump.automatic = prefs.getBool("auto", true);
    config.pump.forceCheck = prefs.getBool("forceCheck", false);
}

void loadConfiguration(Config &config)
{
    Serial.println(F("[Conf] Loading configuration..."));
    if (!prefs.begin("domopool"))
    {
        Serial.println(F("[Conf] Unable to start preferences"));
    }

    boolean init = prefs.getBool("init", false);
    if (!init)
    {
        Serial.println(F("[Conf] Preferences not set; using default."));
        prefs.clear();
        Serial.print("[Conf] free entries: ");
        Serial.println(prefs.freeEntries());
        prefs.putBool("init", true);

        // prefs.putDouble("ack_tone", 4000);
        prefs.putString("ntp_server", "europe.pool.ntp.org");
        prefs.putString("mqtt_server", "192.168.10.194");
        // prefs.putFloat("ph_threshold", 7.4);
    }
    pref2config(config);
    config.states.startup = true;
    Serial.println(F("[Conf] Done"));
}

void config2pref(Config &config)
{
    prefs.putBool("dhcp", config.network.dhcp);
    prefs.putBool("allowPost", config.network.allowPost);
    prefs.putBool("twin_enabled", config.sensors.twin.enabled);
    prefs.putBool("twin_init", config.sensors.twin.init);
    prefs.putBool("twout_init", config.sensors.twout.init);
    prefs.putBool("tamb_init", config.sensors.tamb.init);
    prefs.putBool("waitConvertion", config.sensors.waitForConversion);
    prefs.putInt("tempResolution", config.sensors.tempResolution);
    prefs.putBool("ph_enabled", config.sensors.ph.enabled);
    prefs.putFloat("ph_threshold", config.sensors.ph.threshold);
    prefs.putShort("BacklightTime", config.global.lcdBacklightDuration);
    prefs.putDouble("ack_tone", config.global.ackTone);
    prefs.putInt("ackDuration", config.global.ackDuration);
    prefs.putBool("serialOut", config.global.serialOut);
    prefs.putBool("displayStartup", config.global.displayStartup);
    prefs.putBool("time_init", config.time.initialized);
    prefs.putShort("dayLight", config.time.dayLight);
    prefs.putShort("timeZone", config.time.timeZone);
    prefs.putString("ntp_server", config.time.ntpServer);
    prefs.putBool("forceFilter", config.pump.forceFilter);
    prefs.putBool("forcePH", config.pump.forcePH);
    prefs.putBool("forceCH", config.pump.forceCH);
    prefs.putBool("forceCheck", config.pump.forceCheck);
    prefs.putString("mqtt_server", config.network.mqtt.server);
    prefs.putBool("mqtt_enabled", config.network.mqtt.enabled);
}

void saveConfiguration(Config &config)
{
    Serial.println(F("[Conf] Saving config to preferences"));
    config2pref(config);
    Serial.println(F("[Conf] Done"));
}

void config2doc(Config &config, JsonDocument &doc)
{
    JsonObject jsonObj = doc.to<JsonObject>();
    jsonObj["global"]["lcdBacklightDuration"] = config.global.lcdBacklightDuration;
    jsonObj["global"]["ackTone"] = config.global.ackTone;
    jsonObj["global"]["ackDuration"] = config.global.ackDuration;
    jsonObj["global"]["serialOut"] = config.global.serialOut;
    jsonObj["global"]["displayStartup"] = config.global.displayStartup;
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
    jsonObj["network"]["mqtt"]["server"] = config.network.mqtt.server;
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
    jsonObj["pump"]["forceCheck"] = config.pump.forceCheck;
    jsonObj["pump"]["auto"] = config.pump.automatic;
    jsonObj["tests"]["enabled"] = config.tests.enabled;
    jsonObj["tests"]["tamb"] = config.tests.tamb;
    jsonObj["tests"]["twater"] = config.tests.twater;
    jsonObj["tests"]["pressure"] = config.tests.pressure;
}

void metrics2doc(Config &config, JsonDocument &doc)
{
    doc["metrics"]["chDuration"] = config.metrics.chDuration;
    doc["metrics"]["curCh"] = config.metrics.curCh;
    doc["metrics"]["curPh"] = config.metrics.curPh;
    doc["metrics"]["curTempAmbiant"] = config.metrics.curTempAmbiant;
    doc["metrics"]["curTempWater"] = config.metrics.curTempWater;
    doc["metrics"]["curWaterPressure"] = config.metrics.curWaterPressure;
    doc["metrics"]["hour"] = config.metrics.hour;
    doc["metrics"]["savedTempWater"] = config.metrics.savedTempWater;
}
void states2doc(Config &config, JsonDocument &doc)
{
    doc["states"]["filterOn"] = config.states.filterOn;
    doc["states"]["phOn"] = config.states.phOn;
    doc["states"]["chOn"] = config.states.chOn;
    doc["states"]["automatic"] = config.states.automatic;
    doc["states"]["startup"] = config.states.startup;
}

void initConfigData(Config &config)
{
    config.states.startup = true;
    config.states.filterOn = false;
    config.states.phOn = false;
}

bool stopPump(const int8_t p)
{
    prefs.putBool("auto", false);
    switch (p)
    {
    case 1:
        prefs.putBool("forceFilter", false);
        break;
    case 2:
        prefs.putBool("forceCH", false);
        break;
    case 3:
        prefs.putBool("forcePH", false);
        break;

    default:
        return false;
        break;
    }
    return true;
}
bool startPump(const int8_t p)
{
    prefs.putBool("auto", false);
    switch (p)
    {
    case 1:
        prefs.putBool("forceFilter", true);
        break;
    case 2:
        prefs.putBool("forceCH", true);
        break;
    case 3:
        prefs.putBool("forcePH", true);
        break;

    default:
        return false;
        break;
    }
    return true;
}
bool setPumpAuto()
{
    prefs.putBool("auto", true);
    prefs.putBool("forceCheck", true);
    prefs.putBool("forceFilter", false);
    return true;
}
bool unsetPumpAuto()
{
    prefs.putBool("auto", false);
    return true;
}
void unsetForceCheck()
{
    prefs.putBool("forceCheck", false);
}
void startMqtt()
{
    prefs.putBool("mqtt_enabled", true);
}
void stopMqtt()
{
    prefs.putBool("mqtt_enabled", false);
}
