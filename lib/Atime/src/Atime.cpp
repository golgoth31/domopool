#include "Atime.h"

tm timedata;

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
String printDate()
{
    String date = "";
    date += day();
    date += " ";
    date += monthStr(month());
    date += " ";
    date += year();
    return date;
}

void setSytemTime(bool rtcOk)
{
    Serial.println(F("[Time] Get ntp time"));

    if (getLocalTime(&timedata))
    {
        tmElements_t tmElem;
        tmElem.Day = timedata.tm_mday;
        tmElem.Hour = timedata.tm_hour;
        tmElem.Minute = timedata.tm_min;
        tmElem.Month = timedata.tm_mon + 1;
        tmElem.Second = timedata.tm_sec;
        tmElem.Wday = timedata.tm_wday;
        tmElem.Year = (timedata.tm_year + 1900) - 1970;
        time_t now = makeTime(tmElem);
        setTime(now);
        if (rtcOk)
        {
            Serial.println(F("[Time] Set RTC time"));
            // RTC.set(now);
        }
    }
    else
    {
        Serial.println(F("[Time] Unable to set NTP Time"));
    }
}

time_t getNTPTime()
{
    tmElements_t tmElem;
    getLocalTime(&timedata);
    tmElem.Day = timedata.tm_mday;
    tmElem.Hour = timedata.tm_hour;
    tmElem.Minute = timedata.tm_min;
    tmElem.Month = timedata.tm_mon + 1;
    tmElem.Second = timedata.tm_sec;
    tmElem.Wday = timedata.tm_wday;
    tmElem.Year = (timedata.tm_year + 1900) - 1970;
    return makeTime(tmElem);
}

bool initSystemTime(Time &config)
{
    bool rtcOk;
    configTime(config.timeZone, config.dayLight, config.ntpServer.c_str());
    // if (RTC.chipPresent())
    // {
    //     Serial.println(F("[Time] RTC found, setting..."));
    //     rtcOk = true;
    //     setSyncProvider(RTC.get);
    // }
    // else
    // {
    Serial.println(F("[Time] no rtc module"));
    rtcOk = false;
    setSyncProvider(getNTPTime);
    // }
    setSyncInterval(3600);
    setSytemTime(rtcOk);

    return rtcOk;
}
