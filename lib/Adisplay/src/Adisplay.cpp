#include "Adisplay.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSPI_Button button[2];

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
}

void pageOTA()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setTextSize(2);
    tft.drawCentreString("Firmware update ...", 120, 160, 1);
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
    button[0].initButton(&tft, 60, 235, 118, 148, TFT_BLACK, color, TFT_BLACK, "Filter", 2);
    button[0].drawButton();

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
    button[0].initButton(&tft, 180, 193, 118, 84, TFT_BLACK, color, TFT_BLACK, "Ph", 2);
    button[0].drawButton();

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
    button[0].initButton(&tft, 180, 277, 118, 84, TFT_BLACK, color, TFT_BLACK, "Ch", 2);
    button[0].drawButton();

    // tft.fillRoundRect(162, 120, 158, 120, 5, TFT_BLUE);
}
void displayDate(Config &config)
{
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawCentreString(printDate(), 120, 108, 1);
    tft.drawCentreString(printTime(false), 120, 130, 1);
    tft.drawCentreString(config.network.ip, 120, 152, 1);
}
void displayPageMain(Config &config)
{
    tft.fillScreen(TFT_BLACK);

    tft.drawLine(120, 0, 120, 100, TFT_LIGHTGREY);
    tft.drawLine(0, 50, 240, 50, TFT_LIGHTGREY);
    tft.drawLine(0, 100, 240, 100, TFT_LIGHTGREY);

    tft.drawLine(120, 170, 120, 320, TFT_LIGHTGREY);
    tft.drawLine(0, 170, 240, 170, TFT_LIGHTGREY);

    displayTemp(config);
    displayPump(config);
    displayDate(config);
}
