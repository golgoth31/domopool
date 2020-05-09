#include "Atime.h"

EthernetUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");

String printTime(bool seconds)
{
    String time = "";
    if (hour() < 10)
    {
        time += "0";
    }
    time += hour();
    time += "h";
    if (minute() < 10)
    {
        time += "0";
    }
    time += minute();
    if (seconds)
    {
        time += "m";
        if (second() < 10)
        {
            time += "0";
        }
        time += second();
        time += "s";
    }
    return time;
}

void setSytemTime(bool ethServerStarted)
{
    if (ethServerStarted)
    {
        Serial.println(F("[Time] get ntp time"));
        timeClient.update();
        RTC.set(timeClient.getEpochTime());
    }
    else
    {
        // to be replaced by LCD Hour selection
        // request on lcd screen; wait for 10min to be filled
        // if not, start with compiletime (at least we got something)
        tmElements_t tm;
        if (!RTC.read(tm))
        {
            Serial.println(F("[Time] setup rtc with compile time"));
            RTC.write(tm);
        }
    }
    // Serial.print("timestamp: ");
    // Serial.println(RTC.get());
}

void initSystemTime(Time &config, bool ethServerStarted)
{
    if (RTC.chipPresent)
    {
        Serial.println(F("[Time] RTC found, setting..."));
        if (ethServerStarted)
        {
            String ntpServ = config.ntpServer;
            Serial.print(F("[Time] requesting '"));
            Serial.print(ntpServ);
            Serial.println(F("'"));
            timeClient.setTimeOffset(config.timeZone * 3600);
            timeClient.setUpdateInterval(60000);
            timeClient.begin();
        }
        setSytemTime(ethServerStarted);
    }
    else
    {
        Serial.println(F("[Time] no rtc module"));
    }
}
