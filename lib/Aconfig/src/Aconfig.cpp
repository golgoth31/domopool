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
    config.sensors.ph.threshold = prefs.getFloat("ph_threshold", 0);
    config.sensors.ph.threshold_accuracy = prefs.getShort("ph_t_accuracy", 0);
    config.sensors.ph.vmin = prefs.getFloat("ph_vmin", 0);
    config.sensors.ph.vmax = prefs.getFloat("ph_vmax", 0);
    config.sensors.ph.adc_pin = prefs.getShort("ph_adc_pin", 2);
    config.sensors.ch.enabled = prefs.getBool("ch_enabled", false);
    config.sensors.ch.threshold = prefs.getFloat("ch_threshold", 0);
    config.sensors.ch.threshold_accuracy = prefs.getShort("ch_t_accuracy", 0);
    config.sensors.ch.vmin = prefs.getFloat("ch_vmin", 0);
    config.sensors.ch.vmax = prefs.getFloat("ch_vmax", 0);
    config.sensors.ch.adc_pin = prefs.getShort("ch_adc_pin", 1);
    config.sensors.wp.enabled = prefs.getBool("wp_enabled", true);
    config.sensors.wp.threshold = prefs.getFloat("wp_threshold", 0);
    config.sensors.wp.threshold_accuracy = prefs.getShort("wp_t_accuracy", 8);
    config.sensors.wp.vmin = prefs.getFloat("wp_vmin", 0.5);
    config.sensors.wp.vmax = prefs.getFloat("wp_vmax", 4.5);
    config.sensors.wp.adc_pin = prefs.getShort("wp_adc_pin", 3);
    config.sensors.wait_for_conversion = prefs.getBool("waitConvertion", true);
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
    config.pump.force_duration = prefs.getUInt("forceDuration", 0);
    config.pump.force_start_time = prefs.getUInt("forceStartTime", 0);
    config.global.force_light = prefs.getBool("forceLight", false);
    config.limits.ch_min = prefs.getFloat("ch_min", 0);
    config.limits.ch_max = prefs.getFloat("ch_max", 0);
    config.limits.ph_min = prefs.getFloat("ph_min", 0);
    config.limits.ph_max = prefs.getFloat("ph_max", 0);
    config.limits.wp_min = prefs.getFloat("wp_min", 0.2);
    config.limits.wp_max = prefs.getFloat("wp_max", 2);
    config.limits.ch_temp_threshold = prefs.getFloat("ch_t_threshold", 15);
    config.limits.ch_temp_wait_reset = prefs.getFloat("ch_t_wait", 14);
    config.limits.wait_before_ch = prefs.getShort("ch_wait", 72);
    config.limits.wp_0_derive = prefs.getFloat("wp_0_derive", 0.03);
    config.limits.tw_min = prefs.getShort("tw_min", 1);
    config.limits.tw_max = prefs.getFloat("tw_max", 30);
    config.limits.tamb_min = prefs.getFloat("tamb_min", 0);
}

bool initConfig()
{
    Serial.println(F("[Conf] Loading configuration..."));
    if (!prefs.begin("domopool"))
    {
        Serial.println(F("[Conf] Unable to start preferences"));
        return false;
    }
    return true;
}

void loadDefaultConfig(domopool_Config &config)
{
    // domopool_Config_init_zero();
    config.has_alarms = true;
    config.alarms.has_ads1115 = true;
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
    config.sensors.has_wp = true;
    config.has_states = true;
    config.has_tests = true;
    config.has_limits = true;
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
    prefs.putInt("ph_t_accuracy", config.sensors.ph.threshold_accuracy);
    prefs.putFloat("ph_vmin", config.sensors.ph.vmin);
    prefs.putFloat("ph_vmax", config.sensors.ph.vmax);
    prefs.putInt("ph_adc_pin", config.sensors.ph.adc_pin);
    prefs.putBool("ch_enabled", config.sensors.ch.enabled);
    prefs.putFloat("ch_threshold", config.sensors.ch.threshold);
    prefs.putInt("ch_t_accuracy", config.sensors.ch.threshold_accuracy);
    prefs.putFloat("ch_vmin", config.sensors.ch.vmin);
    prefs.putFloat("ch_vmax", config.sensors.ch.vmax);
    prefs.putInt("ch_adc_pin", config.sensors.ch.adc_pin);
    prefs.putBool("wp_enabled", config.sensors.wp.enabled);
    prefs.putFloat("wp_threshold", config.sensors.wp.threshold);
    prefs.putInt("wp_t_accuracy", config.sensors.wp.threshold_accuracy);
    prefs.putFloat("wp_vmin", config.sensors.wp.vmin);
    prefs.putFloat("wp_vmax", config.sensors.wp.vmax);
    prefs.putShort("wp_adc_pin", config.sensors.wp.adc_pin);
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
    prefs.putBool("forceLight", config.global.force_light);
    prefs.putBool("forceCheck", config.pump.force_check);
    prefs.putUInt("forceDuration", config.pump.force_duration);
    prefs.putUInt("forceStartTime", config.pump.force_start_time);
    prefs.putString("mqtt_server", config.network.mqtt.server);
    prefs.putBool("mqtt_enabled", config.network.mqtt.enabled);
    prefs.putFloat("ch_min", config.limits.ch_min);
    prefs.putFloat("ch_max", config.limits.ch_max);
    prefs.putFloat("ph_min", config.limits.ph_min);
    prefs.putFloat("ph_max", config.limits.ph_max);
    prefs.putFloat("wp_min", config.limits.wp_min);
    prefs.putFloat("wp_max", config.limits.wp_max);
    prefs.putFloat("ch_t_threshold", config.limits.ch_temp_threshold);
    prefs.putFloat("ch_t_wait", config.limits.ch_temp_wait_reset);
    prefs.putShort("ch_wait", config.limits.wait_before_ch);
    prefs.putFloat("wp_0_derive", config.limits.wp_0_derive);
    prefs.putShort("tw_min", config.limits.tw_min);
    prefs.putShort("tw_max", config.limits.tw_max);
    prefs.putShort("tamb_min", config.limits.tamb_min);
}

void saveConfiguration(domopool_Config &config)
{
    Serial.println(F("[Conf] Saving config to preferences"));
    config2pref(config);
    Serial.println(F("[Conf] Done"));
}

void initConfigData(domopool_Config &config)
{
    config.states.startup = true;
    config.states.filter_on = false;
    config.states.ph_on = false;
}

bool setPumpDuration(uint32_t duration)
{
    prefs.putUInt("forceDuration", duration);
    prefs.putUInt("forceStartTime", now());
    return true;
}

bool stopPump(const int8_t p)
{
    switch (p)
    {
    case 1:
        prefs.putBool("auto", false);
        prefs.putBool("forceFilter", false);
        setPumpDuration(0);
        break;
    case 2:
        prefs.putBool("auto", false);
        prefs.putBool("forceCH", false);
        break;
    case 3:
        prefs.putBool("auto", false);
        prefs.putBool("forcePH", false);
        break;
    case 4:
        prefs.putBool("forceLight", false);
        break;

    default:
        return false;
        break;
    }
    return true;
}

bool startPump(const int8_t p, uint32_t duration)
{
    switch (p)
    {
    case 1:
        prefs.putBool("auto", false);
        prefs.putBool("forceFilter", true);
        setPumpDuration(duration);
        break;
    case 2:
        prefs.putBool("auto", false);
        prefs.putBool("forceFilter", true);
        prefs.putBool("forceCH", true);
        break;
    case 3:
        prefs.putBool("auto", false);
        prefs.putBool("forceFilter", true);
        prefs.putBool("forcePH", true);
        break;
    case 4:
        prefs.putBool("forceLight", true);
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
    setPumpDuration(0);
    return true;
}

bool unsetPumpAuto()
{
    prefs.putBool("auto", false);
    prefs.putInt("forceDuration", 0);
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
    Serial.println(F("[WiFi] Rebooting system"));
    esp_restart();
}

void setWP(int adc_pin, float threshold, int taccuracy, float vmin, float vmax)
{
    prefs.putFloat("wp_threshold", threshold);
    prefs.putInt("wp_t_accuracy", taccuracy);
    prefs.putInt("wp_adc_pin", adc_pin);
    prefs.putFloat("wp_vmin", vmin);
    prefs.putFloat("wp_vmax", vmax);
}

void enableWP()
{
    prefs.putBool("wp_enabled", true);
}

void disableWP()
{
    prefs.putBool("wp_enabled", false);
}

void setPH(int adc_pin, float threshold, int taccuracy, float vmin, float vmax)
{
    prefs.putBool("ph_enabled", true);
    prefs.putFloat("ph_threshold", threshold);
    prefs.putInt("ph_t_accuracy", taccuracy);
    prefs.putInt("ph_adc_pin", adc_pin);
    prefs.putFloat("ph_vmin", vmin);
    prefs.putFloat("ph_vmax", vmax);
}

void disablePH()
{
    prefs.putBool("ph_enabled", false);
}

void setCH(int adc_pin, float threshold, int taccuracy, float vmin, float vmax)
{
    prefs.putBool("ch_enabled", true);
    prefs.putFloat("ch_threshold", threshold);
    prefs.putInt("ch_t_accuracy", taccuracy);
    prefs.putInt("ch_adc_pin", adc_pin);
    prefs.putFloat("ch_vmin", vmin);
    prefs.putFloat("ch_vmax", vmax);
}

void disableCH()
{
    prefs.putBool("ch_enabled", false);
}
