#include "Atime.h"

tm timedata;
RTC_DS1307 rtc;

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

void setSytemTime(bool rtcOk)
{
    Serial.println(F("[Time] Get ntp time"));
    if (getLocalTime(&timedata))
    {
        time_t now;
        time(&now);
        if (rtcOk)
        {
            Serial.println(F("[Time] Set RTC time"));
            rtc.adjust(
                DateTime(
                    timedata.tm_year,
                    timedata.tm_mon,
                    timedata.tm_mday,
                    timedata.tm_hour,
                    timedata.tm_min,
                    timedata.tm_sec));
        }
    }
    else
    {
        Serial.println(F("[Time] Unable to get ntp time, using RTC"));
        DateTime dt = rtc.now();
        const timeval tv = {dt.unixtime, 0};
        settimeofday(&tv, );
    }
}

void initSystemTime(Time &config)
{
    bool rtcOk;
    configTime(config.timeZone, config.dayLight, config.ntpServer);
    if (rtc.begin())
    {
        Serial.println(F("[Time] RTC found, setting..."));
        rtcOk = true;
    }
    else
    {
        Serial.println(F("[Time] no rtc module"));
        rtcOk = false;
    }
    setSytemTime(rtcOk);
}
