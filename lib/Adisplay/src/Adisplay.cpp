#include "Adisplay.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSPI_Button button[2];

#define BUTTON_BOX_X 0
#define BUTTON_BOX_Y 170
#define BUTTON_BOX_W 240
#define BUTTON_BOX_H 150

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

void initDisplay()
{
    tft.init();
    tft.setRotation(0);

    uint16_t calData[5] = {251, 3545, 202, 3420, 2};
    tft.setTouch(calData);

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_DARKCYAN);
    tft.setTextSize(2);
    tft.drawCentreString("Domopool startup !", 120, 160, 1);

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
}

void pageOTA()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawCentreString("Firmware update", 120, 150, 1);
}

void pageOTAdot(int8_t dot)
{

    switch (dot)
    {
    case 1:
        tft.drawCentreString("    .    ", 120, 170, 1);
        break;
    case 2:
        tft.drawCentreString("   ...   ", 120, 170, 1);
        break;
    case 3:
        tft.drawCentreString("  .....  ", 120, 170, 1);
        break;
    case 4:
        tft.drawCentreString(" ....... ", 120, 170, 1);
        break;
    case 5:
        tft.drawCentreString(".........", 120, 170, 1);
        break;

    default:
        break;
    }
}

void displayPageBoot()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
}

void display2boot(String text, boolean serialOut)
{
    tft.println(text);
    if (serialOut)
    {
        Serial.println(text);
    }
}

void displayTemp(Config &config)
{
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    // tft.setFreeFont(FF17);

    String text = "";
    text += config.metrics.curTempWater;
    text += (char)247;
    text += "C";
    tft.drawString("Water", 60, 15, 1);
    tft.drawString(text, 60, 37, 1);

    text = "";
    text += config.metrics.curTempAmbiant;
    text += (char)247;
    text += "C";
    tft.drawString("Ambiant", 180, 15, 1);
    tft.drawString(text, 180, 37, 1);

    text = "";
    text += config.metrics.curPh;
    tft.drawString("Ph", 60, 65, 1);
    tft.drawString(text, 60, 87, 1);

    text = "";
    text += config.metrics.curWaterPressure;
    text += " Bar";
    tft.drawString("Pressure", 180, 65, 1);
    tft.drawString(text, 180, 87, 1);
}
void displayPump(Config &config)
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
    if (config.metrics.filterOn)
    {
        color = TFT_GREEN;
    }
    else if (config.metrics.alarms.filter)
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

    if (config.metrics.phOn)
    {
        color = TFT_GREEN;
    }
    else if (config.metrics.alarms.ph)
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

    if (config.metrics.chOn)
    {
        color = TFT_GREEN;
    }
    else if (config.metrics.alarms.ch)
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
void displayDate(Config &config)
{
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(printDate(), 0, 115, 1);
    tft.drawString(printTime(false), 0, 137, 1);
    tft.drawString(config.network.ip, 0, 159, 1);
}
void displayServices(Config &config)
{
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);

    if (config.network.active)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    tft.drawRightString("NET", 240, 107, 1);

    if (config.network.mqtt.enabled)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else
    {
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    }
    tft.drawRightString("MQTT", 240, 129, 1);

    if (config.time.initialized)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    tft.drawRightString("TIME", 240, 150, 1);
}
void displayPageMain(Config &config)
{
    tft.fillScreen(TFT_BLACK);

    tft.drawLine(120, 0, 120, 100, TFT_LIGHTGREY);
    tft.drawLine(0, 50, 240, 50, TFT_LIGHTGREY);
    tft.drawLine(0, 100, 240, 100, TFT_LIGHTGREY);

    // tft.drawLine(120, 170, 120, 320, TFT_LIGHTGREY);
    // tft.drawLine(0, 170, 240, 170, TFT_LIGHTGREY);
    // tft.drawLine(0, 245, 240, 245, TFT_LIGHTGREY);

    displayTemp(config);
    displayPump(config);
    displayDate(config);
}
void displayPressed(Config &config)
{
    uint16_t x, y;

    // See if there's any touch data for us
    boolean pressed = tft.getTouch(&x, &y);

    if (pressed)
    {
        Serial.println("touched");
        if ((x > FILTER_X) && (x < FILTER_X + FILTER_W))
        {
            if ((y > FILTER_Y) && (y <= FILTER_Y + FILTER_H))
            {
                Serial.println("filter");
                if (config.metrics.filterOn)
                {
                    stopPump(1);
                }
                else
                {
                    startPump(1);
                }
            }
        }
        if ((x > AUTO_X) && (x < AUTO_X + AUTO_W))
        {
            if ((y > AUTO_Y) && (y <= AUTO_Y + AUTO_H))
            {
                Serial.println("auto");
                if (config.pump.automatic)
                {
                    unsetPumpAuto();
                }
                else
                {
                    setPumpAuto();
                }
            }
        }
    }
}
