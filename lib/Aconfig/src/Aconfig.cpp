#include "Aconfig.h"

const int ConfigDocSize = 1024;
const char *networkFile = "/network.jsn";
Preferences prefs;

void pref2config(domopool_Config &config)
{
    String defaultNtpServer = "europe.pool.ntp.org";
    String defaultMQTTServer = "192.168.10.194";
    // double_t defaultAckTone = 4000;
    // float_t defaultPhThreshold = 7.4;

    //default not working
    config.global.ack_tone = prefs.getDouble("ack_tone", 4000);
    strcpy(config.network.ntp.server, prefs.getString("ntp_server", defaultNtpServer).c_str());
    strcpy(config.network.mqtt.server, prefs.getString("mqtt_server", defaultMQTTServer).c_str());
    config.sensors.ph.threshold = prefs.getFloat("ph_threshold", 7.4);

    //no bug
    config.global.lcd_backlight_duration = prefs.getShort("BacklightTime", 30000);
    config.global.ack_duration = prefs.getInt("ackDuration", 100);
    config.global.serial_out = prefs.getBool("serialOut", true);
    config.global.display_startup = prefs.getBool("displayStartup", false);
    config.network.ntp.day_light = prefs.getShort("dayLight", 3600);
    config.network.ntp.timezone = prefs.getShort("timeZone", 3600);
    config.network.dhcp = prefs.getBool("dhcp", true);
    config.network.allow_post = prefs.getBool("allowPost", true);
    config.network.mqtt.enabled = prefs.getBool("mqtt_enabled", false);
    config.sensors.ph.enabled = prefs.getBool("ph_enabled", false);
    config.sensors.wait_for_conversion = prefs.getBool("waitConvertion", false);
    config.sensors.temp_resolution = prefs.getShort("tempResolution", 12);
    config.sensors.twin.enabled = prefs.getBool("twin_enabled", false);
    config.sensors.twin.init = prefs.getBool("twin_init", false);
    config.sensors.twout.enabled = true;
    config.sensors.twout.init = prefs.getBool("twout_init", false);
    config.sensors.tamb.enabled = true;
    config.sensors.tamb.init = prefs.getBool("tamb_init", false);
    config.pump.force_filter = prefs.getBool("forceFilter", false);
    config.pump.force_ph = prefs.getBool("forcePH", false);
    config.pump.force_ch = prefs.getBool("forceCH", false);
    config.pump.automatic = prefs.getBool("auto", true);
    config.pump.force_check = prefs.getBool("forceCheck", false);
}

void loadConfiguration(domopool_Config &config)
{
    Serial.println(F("[Conf] Loading configuration..."));
    if (!prefs.begin("domopool"))
    {
        Serial.println(F("[Conf] Unable to start preferences"));
    }

    // domopool_Config_init_zero();
    config.has_alarms = true;
    config.has_global = true;
    config.has_metrics = true;
    config.has_network = true;
    config.network.has_mqtt = true;
    config.network.has_ntp = true;
    config.has_pump = true;
    config.has_sensors = true;
    config.sensors.has_ch = true;
    config.sensors.has_ph = true;
    config.sensors.has_tamb = true;
    config.sensors.has_twin = true;
    config.sensors.has_twout = true;
    config.sensors.has_water_pressure = true;
    config.has_states = true;
    config.has_tests = true;
    boolean init = prefs.getBool("init", false);
    if (!init)
    {
        Serial.println(F("[Conf] Preferences not set; using default."));
        prefs.clear();
        Serial.print("[Conf] free entries: ");
        Serial.println(prefs.freeEntries());
        prefs.putBool("init", true);

        prefs.putDouble("ack_tone", 4000);
        prefs.putString("ntp_server", "europe.pool.ntp.org");
        prefs.putString("mqtt_server", "192.168.10.194");
        prefs.putFloat("ph_threshold", 7.4);
    }
    pref2config(config);
    config.states.startup = true;
    Serial.println(F("[Conf] Done"));
}

void config2pref(domopool_Config &config)
{
    prefs.putBool("dhcp", config.network.dhcp);
    prefs.putBool("allowPost", config.network.allow_post);
    prefs.putBool("twin_enabled", config.sensors.twin.enabled);
    prefs.putBool("twin_init", config.sensors.twin.init);
    prefs.putBool("twout_init", config.sensors.twout.init);
    prefs.putBool("tamb_init", config.sensors.tamb.init);
    prefs.putBool("waitConvertion", config.sensors.wait_for_conversion);
    prefs.putInt("tempResolution", config.sensors.temp_resolution);
    prefs.putBool("ph_enabled", config.sensors.ph.enabled);
    prefs.putFloat("ph_threshold", config.sensors.ph.threshold);
    prefs.putShort("BacklightTime", config.global.lcd_backlight_duration);
    prefs.putDouble("ack_tone", config.global.ack_tone);
    prefs.putInt("ackDuration", config.global.ack_duration);
    prefs.putBool("serialOut", config.global.serial_out);
    prefs.putBool("displayStartup", config.global.display_startup);
    prefs.putShort("dayLight", config.network.ntp.day_light);
    prefs.putShort("timeZone", config.network.ntp.timezone);
    prefs.putString("ntp_server", config.network.ntp.server);
    prefs.putBool("forceFilter", config.pump.force_filter);
    prefs.putBool("forcePH", config.pump.force_ph);
    prefs.putBool("forceCH", config.pump.force_ch);
    prefs.putBool("forceCheck", config.pump.force_check);
    prefs.putString("mqtt_server", config.network.mqtt.server);
    prefs.putBool("mqtt_enabled", config.network.mqtt.enabled);
}

void saveConfiguration(domopool_Config &config)
{
    Serial.println(F("[Conf] Saving config to preferences"));
    config2pref(config);
    Serial.println(F("[Conf] Done"));
}

void metrics2doc(domopool_Config &config, JsonDocument &doc)
{
    doc["metrics"]["over15Duration"] = config.metrics.over_15_duration;
    doc["metrics"]["ch"] = config.metrics.ch;
    doc["metrics"]["ph"] = config.metrics.ph;
    doc["metrics"]["tempAmbiant"] = config.metrics.tamb;
    doc["metrics"]["tempWater"] = config.metrics.twater;
    doc["metrics"]["waterPressure"] = config.metrics.water_pressure;
    doc["metrics"]["hour"] = config.metrics.hour;
    doc["metrics"]["savedTempWater"] = config.metrics.saved_twater;
}
void states2doc(domopool_Config &config, JsonDocument &doc)
{
    doc["states"]["filterOn"] = config.states.filter_on;
    doc["states"]["phOn"] = config.states.ph_on;
    doc["states"]["chOn"] = config.states.ch_on;
    doc["states"]["automatic"] = config.states.automatic;
    doc["states"]["startup"] = config.states.startup;
    doc["states"]["ntp"] = config.states.ntp;
    doc["states"]["rtc"] = config.states.rtc;
}

void initConfigData(domopool_Config &config)
{
    config.states.startup = true;
    config.states.filter_on = false;
    config.states.ph_on = false;
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

void resetConfig()
{
    prefs.putBool("init", false);
}

void reboot()
{
    if (!prefs.getBool("init", true))
    {
        Serial.println(F("[WiFi] Rebooting system"));
        esp_restart();
    }
}
