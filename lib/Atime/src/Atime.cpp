#include "Atime.h"

EthernetUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");

String printTime()
{
    String time = "";
    if (hour() < 10)
    {
        time += "0";
    }
    time += hour();
    time += ":";
    if (minute() < 10)
    {
        time += "0";
    }
    time += minute();
    time += ":";
    if (second() < 10)
    {
        time += "0";
    }
    time += second();
    return time;
}

void setSytemTime(bool ethServerStarted)
{
    if (ethServerStarted)
    {
        Serial.println("[Time] get ntp time");
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
            Serial.println("[Time] setup rtc with compile time");
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
        Serial.println("[Time] RTC found, setting...");
        if (ethServerStarted)
        {
            String ntpServ = config.ntpServer;
            Serial.print("[Time] requesting '");
            Serial.print(ntpServ);
            Serial.println("'");
            timeClient.setTimeOffset(config.timeZone * 3600);
            timeClient.setUpdateInterval(60000);
            timeClient.begin();
        }
        setSytemTime(ethServerStarted);
    }
    else
    {
        Serial.println("no rtc module");
    }
}
