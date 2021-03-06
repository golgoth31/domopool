#include "Adisplay.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSPI_Button button[2];

#define BUTTON_BOX_X 0
#define BUTTON_BOX_Y 170
#define BUTTON_BOX_W 240
#define BUTTON_BOX_H 150

#define SENSORS_BOX_X 0
#define SENSORS_BOX_Y 0
#define SENSORS_BOX_W 120
#define SENSORS_BOX_H 50

int pBarw = 220;

int FILTER_X;
int FILTER_Y;
int FILTER_W;
int FILTER_H;
int FILTER_TEXT_X;
int FILTER_TEXT_Y;
int AUTO_X;
int AUTO_Y;
int AUTO_W;
int AUTO_H;
int AUTO_TEXT_X;
int AUTO_TEXT_Y;
int PH_X;
int PH_Y;
int PH_W;
int PH_H;
int PH_TEXT_X;
int PH_TEXT_Y;
int CH_X;
int CH_Y;
int CH_W;
int CH_H;
int CH_TEXT_X;
int CH_TEXT_Y;
int WP_X;
int WP_Y;
int WP_W;
int WP_H;
int LINE_H1_XS;
int LINE_H1_YS;
int LINE_H1_XE;
int LINE_H1_YE;
int LINE_V1_XS;
int LINE_V1_YS;
int LINE_V1_XE;
int LINE_V1_YE;
int LINE_H2_XS;
int LINE_H2_YS;
int LINE_H2_XE;
int LINE_H2_YE;

void Button_ACK_Tone(domopool_Config &config)
{
    ledcWriteTone(0, config.global.ack_tone);
    delay(config.global.ack_duration);
    ledcWriteTone(0, 0);
}

void initDisplay()
{
    tft.init();
    tft.setRotation(0);

    uint16_t calData[5] = {251, 3545, 202, 3420, 2};
    tft.setTouch(calData);

    // tft.fillScreen(TFT_BLACK);
    // tft.setTextColor(TFT_DARKCYAN);
    // tft.setTextSize(2);
    // tft.drawCentreString("Domopool startup !", 120, 160, 1);
    // delay(1000);
    String startup = "Domopool startup";
    displayProgressBarText(startup, TFT_CYAN);

    ledcSetup(0, 1E5, 12);
    ledcAttachPin(21, 0);

    AUTO_X = BUTTON_BOX_X + 1;
    AUTO_Y = BUTTON_BOX_Y + 2;
    AUTO_W = (BUTTON_BOX_W / 2) - 2;
    AUTO_H = (BUTTON_BOX_H / 2) - 2;
    AUTO_TEXT_X = AUTO_X + (AUTO_W / 2);
    AUTO_TEXT_Y = AUTO_Y + (AUTO_H / 2);

    FILTER_X = AUTO_X;
    FILTER_Y = AUTO_Y + AUTO_H + 2;
    FILTER_W = AUTO_W;
    FILTER_H = AUTO_H;
    FILTER_TEXT_X = FILTER_X + (FILTER_W / 2);
    FILTER_TEXT_Y = FILTER_Y + (FILTER_H / 2);

    PH_X = BUTTON_BOX_X + (BUTTON_BOX_W / 2) + 1;
    PH_Y = BUTTON_BOX_Y + 2;
    PH_W = (BUTTON_BOX_W / 2) - 2;
    PH_H = (BUTTON_BOX_H / 2) - 2;
    PH_TEXT_X = PH_X + (PH_W / 2);
    PH_TEXT_Y = PH_Y + (PH_H / 2);

    CH_X = PH_X;
    CH_Y = PH_Y + PH_H + 2;
    CH_W = PH_W;
    CH_H = PH_H;
    CH_TEXT_X = CH_X + (CH_W / 2);
    CH_TEXT_Y = CH_Y + (CH_H / 2);

    LINE_H1_XS = SENSORS_BOX_X;
    LINE_H1_YS = SENSORS_BOX_H;
    LINE_H1_XE = SENSORS_BOX_W * 2;
    LINE_H1_YE = SENSORS_BOX_H;

    LINE_H2_XS = SENSORS_BOX_X;
    LINE_H2_YS = SENSORS_BOX_H * 2;
    LINE_H2_XE = SENSORS_BOX_W * 2;
    LINE_H2_YE = SENSORS_BOX_H * 2;

    LINE_V1_XS = SENSORS_BOX_W;
    LINE_V1_YS = SENSORS_BOX_Y;
    LINE_V1_XE = SENSORS_BOX_W;
    LINE_V1_YE = SENSORS_BOX_H * 2;

    WP_X = SENSORS_BOX_W;
    WP_Y = SENSORS_BOX_H;
    WP_W = SENSORS_BOX_W;
    WP_H = SENSORS_BOX_H;
}

// void pageOTA(String type)
// {
//     type += " update";
//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_RED, TFT_BLACK);
//     tft.setTextSize(2);
//     tft.drawCentreString(type, 120, 150, 1);
//     tft.drawRect(10, 170, pBarw, 15, TFT_RED);
// }
// void pageOTAProgressBar(int percent)
// {
//     int width = 0;
//     if (percent > 0)
//     {
//         width = (pBarw * percent) / 100;
//     }
//     tft.fillRect(10, 170, width, 15, TFT_RED);
// }
void displayProgressBarText(String text, uint32_t color)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(color, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawCentreString(text, 120, 150, 1);
    tft.drawRect(10, 170, pBarw, 15, color);
}
void displayProgressBar(int percent, uint32_t color)
{
    int width = 0;
    if (percent > 0)
    {
        width = (pBarw * percent) / 100;
    }
    tft.fillRect(10, 170, width, 15, color);
}

void displayPageBoot()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
}

void display2boot(String text, domopool_Config &config)
{
    if (config.global.display_startup)
    {
        tft.println(text);
    }
    if (config.global.serial_out)
    {
        Serial.println(text);
    }
}

void displaySensors(domopool_Config &config)
{

    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Water", 60, 15, 1);
    if (config.metrics.twater < 2)
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    else if (config.metrics.twater <= 15)
    {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    }
    else
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }

    String text = "";
    if (config.metrics.twater >= 0 && config.metrics.twater < 10)
    {
        text = " ";
    }
    text += config.metrics.twater;
    text += (char)247;
    text += "C";
    tft.drawString(text, 60, 37, 1);

    if (config.metrics.twater >= 0 && config.metrics.twater < 10)
    {
        text = " ";
    }
    else
    {
        text = "";
    }
    text += config.metrics.tamb;
    text += (char)247;
    text += "C";
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Ambiant", 180, 15, 1);
    tft.drawString(text, 180, 37, 1);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Ph", 60, 65, 1);
    if (config.metrics.ph < 7 || config.metrics.ph > 7.8)
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    else if (config.metrics.ph <= 7.3 || config.metrics.ph >= 7.5)
    {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    }
    else if (config.metrics.ph > 7.3 || config.metrics.ph < 7.5)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    text = "";
    text += config.metrics.ph;
    tft.drawString(text, 60, 87, 1);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Pressure", 180, 65, 1);
    if (config.sensors.wp.enabled)
    {
        if (config.states.filter_on)
        {
            if (config.metrics.wp <= config.limits.wp_min || config.metrics.wp > config.limits.wp_max)
            {
                tft.setTextColor(TFT_RED, TFT_BLACK);
            }
            else if (config.metrics.wp > config.limits.wp_min || config.metrics.wp <= config.limits.wp_max)
            {
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
            }
        }
        else
        {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
    }
    else
    {
        tft.setTextColor(TFT_CYAN, TFT_BLACK);
    }

    if (config.metrics.wp >= 0)
    {
        text = " ";
    }
    else
    {
        text = "";
    }
    text += config.metrics.wp;
    text += " Bar";
    tft.drawString(text, 180, 87, 1);
}

void displayRelay(domopool_Config &config)
{
    // printText(0, 50, "Pump:", 1, 1);
    int color;

    // filter auto
    if (config.pump.automatic)
    {
        color = TFT_GREEN;
    }
    else
    {
        color = TFT_RED;
    }
    tft.fillRoundRect(AUTO_X, AUTO_Y, AUTO_W, AUTO_H, 10, color);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Auto", AUTO_TEXT_X, AUTO_TEXT_Y);

    // filter button
    if (config.states.filter_on)
    {
        color = TFT_GREEN;
    }
    else if (config.alarms.filter)
    {
        color = TFT_RED;
    }
    else
    {
        color = TFT_CYAN;
    }
    tft.fillRoundRect(FILTER_X, FILTER_Y, FILTER_W, FILTER_H, 10, color);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Filter", FILTER_TEXT_X, FILTER_TEXT_Y);

    if (config.states.ph_on)
    {
        color = TFT_GREEN;
    }
    else if (config.alarms.ph)
    {
        color = TFT_RED;
    }
    else
    {
        color = TFT_CYAN;
    }
    tft.fillRoundRect(PH_X, PH_Y, PH_W, PH_H, 10, color);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Ph", PH_TEXT_X, PH_TEXT_Y);

    if (config.states.ch_on)
    {
        color = TFT_GREEN;
    }
    else if (config.alarms.ch)
    {
        color = TFT_RED;
    }
    else
    {
        color = TFT_CYAN;
    }
    tft.fillRoundRect(CH_X, CH_Y, CH_W, CH_H, 10, color);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Ch", CH_TEXT_X, CH_TEXT_Y);
}
void displayDate(domopool_Config &config)
{
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(CL_DATUM);
    tft.drawString(printDate(), 0, 115, 1);
    tft.drawString(printTime(false), 0, 137, 1);
}

void displayWifi(domopool_Config &config, bool connecting)
{
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(CL_DATUM);
    String wifiText = config.network.ip;
    if (connecting)
    {
        wifiText = "Connecting";
    }
    tft.drawString(wifiText, 0, 159, 1);
}

void displayServices(domopool_Config &config)
{
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);

    if (config.states.net_active)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    tft.drawRightString("NET", 240, 107, 1);

    if (config.states.mqtt_connected)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else
    {
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    }
    tft.drawRightString("MQTT", 240, 129, 1);

    String time = "TIME";
    if (config.states.rtc && config.states.ntp)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else if (config.states.ntp)
    {
        tft.setTextColor(TFT_GOLD, TFT_BLACK);
        time = "NTP";
    }
    else if (config.states.rtc)
    {
        tft.setTextColor(TFT_CYAN, TFT_BLACK);
        time = "RTC";
    }
    else
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    tft.drawRightString(time, 240, 150, 1);
}
void displayPageMain(domopool_Config &config)
{
    tft.fillScreen(TFT_BLACK);

    tft.drawLine(LINE_V1_XS, LINE_V1_YS, LINE_V1_XE, LINE_V1_YE, TFT_WHITE);
    tft.drawLine(LINE_H1_XS, LINE_H1_YS, LINE_H1_XE, LINE_H1_YE, TFT_WHITE);
    tft.drawLine(LINE_H2_XS, LINE_H2_YS, LINE_H2_XE, LINE_H2_YE, TFT_WHITE);
}

void displayPressed(domopool_Config &config)
{
    uint16_t x, y;

    // See if there's any touch data for us
    boolean pressed = tft.getTouch(&x, &y);

    if (pressed && !config.states.startup)
    {
        Serial.print("touched ");
        Button_ACK_Tone(config);

        // filter button
        if ((x > FILTER_X) && (x < FILTER_X + FILTER_W))
        {
            if ((y > FILTER_Y) && (y <= FILTER_Y + FILTER_H))
            {
                Serial.println("filter");
                if (config.states.filter_on)
                {
                    stopRelay(domopool_Relay_names_filter);
                }
                else
                {
                    startRelay(domopool_Relay_names_filter, 0);
                }
            }
        }

        // auto button
        if ((x > AUTO_X) && (x < AUTO_X + AUTO_W))
        {
            if ((y > AUTO_Y) && (y <= AUTO_Y + AUTO_H))
            {
                Serial.println("auto");
                if (config.states.automatic)
                {
                    unsetRelayAuto();
                }
                else
                {
                    setRelayAuto();
                }
            }
        }

        // ch button
        if ((x > CH_X) && (x < CH_X + CH_W))
        {
            if ((y > CH_Y) && (y <= CH_Y + CH_H))
            {
                Serial.println("ch");
                if (!config.pump.automatic)
                {
                    if (config.states.ch_on)
                    {
                        stopRelay(domopool_Relay_names_ch);
                    }
                    else
                    {
                        startRelay(domopool_Relay_names_ch, 0);
                    }
                }
                else
                {
                    forceChDuration(config);
                }
            }
        }

        // water pressure activation
        if ((x > WP_X) && (x < WP_X + WP_W))
        {
            if ((y > WP_Y) && (y <= WP_Y + WP_H))
            {
                Serial.println("wp");
                if (config.sensors.wp.enabled)
                {
                    disableWP();
                }
                else
                {
                    enableWP();
                }
            }
        }
    }
}
