#include "Atime.h"

tm timedata;
RtcDateTime rdt;

RtcDS3231<TwoWire>
    RTC(Wire);
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

void setSytemTime(domopool_Config &config)
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
        if (config.states.rtc)
        {
            Serial.println(F("[Time] Set RTC time"));
            RTC.SetDateTime(RtcDateTime(
                timedata.tm_year + 1900,
                tmElem.Month,
                tmElem.Day,
                tmElem.Hour,
                tmElem.Minute,
                tmElem.Second));
        }
        config.states.ntp = true;
    }
    else
    {
        Serial.println(F("[Time] Unable to set NTP Time"));
        config.states.ntp = false;
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
time_t getRTCTime()
{
    tmElements_t tmElem;
    rdt = RTC.GetDateTime();
    tmElem.Day = rdt.Day();
    tmElem.Hour = rdt.Hour();
    tmElem.Minute = rdt.Minute();
    tmElem.Month = rdt.Month();
    tmElem.Second = rdt.Second();
    tmElem.Wday = rdt.DayOfWeek();
    tmElem.Year = rdt.Year() - 1970;
    return makeTime(tmElem);
}
time_t getCurrentTime()
{
    tmElements_t tmElem;
    tmElem.Day = day();
    tmElem.Hour = hour();
    tmElem.Minute = minute();
    tmElem.Month = month();
    tmElem.Second = second();
    tmElem.Wday = weekday();
    tmElem.Year = year() - 1970;
    return makeTime(tmElem);
}

void initSystemTime(domopool_Config &config, int sda, int scl)
{
    configTime(
        config.network.ntp.timezone,
        config.network.ntp.day_light,
        config.network.ntp.server);
    // Wire.begin(35, 34, 400000);
    RTC.Begin(sda, scl);
    if (!RTC.GetIsRunning() || RTC.LastError() == 0)
    {
        RTC.SetIsRunning(true);
    }
    if (RTC.LastError() != 0)
    {
        Serial.println(F("[Time] no rtc module"));
        Serial.print(F("[Time] RTC error: "));
        Serial.println(RTC.LastError());
        config.states.rtc = false;
        setSyncProvider(getNTPTime);
    }
    else
    {
        Serial.println(F("[Time] RTC found"));
        RTC.Enable32kHzPin(false);
        RTC.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
        config.states.rtc = true;
        setSyncProvider(getRTCTime);
    }
    // setSyncInterval(3600);
    setSytemTime(config);
}
