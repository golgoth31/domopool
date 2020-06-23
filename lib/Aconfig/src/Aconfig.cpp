#include "Aconfig.h"

const int ConfigDocSize = 1024;
const char *networkFile = "/network.jsn";
Preferences prefs;

void pref2config(Config &config)
{
    config.global.lcdBacklightDuration = prefs.getShort("BacklightTime");
    config.time.initialized = prefs.getBool("time_init");
    config.time.dayLight = prefs.getShort("dayLight");
    config.time.ntpServer = prefs.getString("ntpServer");
    config.time.timeZone = prefs.getShort("timeZone");
    config.network.dhcp = prefs.getBool("dhcp");
    config.network.allowPost = prefs.getBool("allowPost");
    config.network.mqtt.enabled = prefs.getBool("mqtt_enabled");
    config.network.mqtt.server = prefs.getString("mqtt_server");
    config.sensors.ph.enabled = prefs.getBool("ph_enabled");
    config.sensors.ph.threshold = prefs.getFloat("ph_threshold");
    config.sensors.waitForConversion = prefs.getBool("waitConvertion");
    config.sensors.tempResolution = prefs.getShort("tempResolution");
    config.sensors.twin.enabled = prefs.getBool("twin_enabled");
    config.sensors.twin.init = prefs.getBool("twin_init");
    config.sensors.twout.enabled = true;
    config.sensors.twout.init = prefs.getBool("twout_init");
    config.sensors.tamb.enabled = true;
    config.sensors.tamb.init = prefs.getBool("tamb_init");
    config.pump.forceFilter = prefs.getBool("forceFilter");
    config.pump.forcePH = prefs.getBool("forcePH");
    config.pump.forceCH = prefs.getBool("forceCH");
}

void loadConfiguration(Config &config)
{
    display2boot(F("[Conf] Loading configuration..."), true);
    prefs.begin("domopool");
    boolean init = prefs.getBool("init");
    if (!init)
    {
        display2boot(F("[Conf] Preferences not set; setting default."), true);
        prefs.clear();
        prefs.putBool("init", true);
        prefs.putBool("dhcp", true);
        prefs.putBool("allowPost", true);
        prefs.putBool("twin_enabled", false);
        prefs.putBool("twin_init", false);
        prefs.putBool("twout_init", false);
        prefs.putBool("tamb_init", false);
        prefs.putBool("waitConvertion", false);
        prefs.putShort("tempResolution", 12);
        prefs.putBool("ph_enabled", false);
        prefs.putFloat("ph_threshold", 7.4);
        prefs.putShort("BacklightTime", 30000);
        prefs.putBool("time_init", false);
        prefs.putShort("dayLight", 3600);
        prefs.putShort("timeZone", 3600);
        prefs.putString("ntpServer", "europe.pool.ntp.org");
        prefs.putBool("forceFilter", true);
        prefs.putBool("forcePH", false);
        prefs.putBool("forceCH", false);
        prefs.putString("mqtt_server", "192.168.10.194");
        prefs.putBool("mqtt_enabled", true);
    }
    pref2config(config);
    display2boot(F("[Conf] Done"), true);
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
    prefs.putBool("time_init", config.time.initialized);
    prefs.putShort("dayLight", config.time.dayLight);
    prefs.putShort("timeZone", config.time.timeZone);
    prefs.putString("ntpServer", config.time.ntpServer);
    prefs.putBool("forceFilter", config.pump.forceFilter);
    prefs.putBool("forcePH", config.pump.forcePH);
    prefs.putBool("forceCH", config.pump.forceCH);
    prefs.putString("mqtt_server", config.network.mqtt.server);
    prefs.putBool("mqtt_enabled", config.network.mqtt.enabled);
}

void saveConfiguration(Config &config)
{
    display2boot(F("[Conf] Saving config to preferences"), true);
    config2pref(config);
    display2boot(F("[Conf] Done"), true);
}

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

void initConfigData(Config &config)
{
    config.metrics.startup = true;
    config.metrics.filterOn = false;
    config.metrics.phOn = false;
}
