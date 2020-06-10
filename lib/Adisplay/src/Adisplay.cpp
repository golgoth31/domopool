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
    tft.drawString("Water", 60, 10, 1);
    tft.drawString(text, 60, 53, 1);

    text = "";
    text += config.metrics.curTempAmbiant;
    text += (char)247;
    text += "C";
    tft.drawString("Ambiant", 180, 10, 1);
    tft.drawString(text, 180, 53, 1);
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
    // tft.fillRoundRect(0, 120, 158, 120, 5, color);
    // tft.setTextColor(TFT_BLACK, color);
    // printText(0, 180, "Filter", 2, 2, 158);

    button[0].initButton(&tft, 60, 267, 118, 104, TFT_BLACK, color, TFT_BLACK, "Filter", 2);
    button[0].drawButton();

    // tft.fillRoundRect(162, 120, 158, 120, 5, TFT_BLUE);
}
void displayDate()
{
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawCentreString(printDate(), 120, 150, 1);
    tft.drawCentreString(printTime(false), 120, 170, 1);
}
void page1(Config &config)
{
    tft.fillScreen(TFT_BLACK);

    tft.drawLine(120, 0, 120, 320, TFT_LIGHTGREY);
    tft.drawLine(0, 107, 240, 107, TFT_LIGHTGREY);
    tft.drawLine(0, 214, 240, 214, TFT_LIGHTGREY);

    displayTemp(config);
    displayPump(config);
    displayDate();
}
