#include "Adisplay.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSPI_Button button[2];

void printText(int x, int y, String text, uint8_t textSize = 1, uint8_t textAllign = 1, uint16_t lineLength = 319)
{
    /*  This function is used for displaying text on the screen
   *  Arguments:
   *    - X           position of the cursor
   *    - Y           position of the cursor
   *    - text        the actual text that will be displayed
   *    - textSize    text size can be one of these values 1, 2, 3, 4, 5
   *    - textAllign  text allign can be 1 - left align, 2 - center and 3 - right align
   *    - lineLenght  this should be used for line lenght of text, but does not works as shoud - TODO
   *
   *  Returns:
   *  nothing
   */

    uint8_t newTextSize = textSize;
    uint8_t real_x = 0;
    uint32_t stringLength = text.length();
    uint8_t characters = stringLength * 5 * newTextSize + stringLength * newTextSize;

    while ((characters + 10) > lineLength)
    {
        // make text smaller if it exceeds the screen
        // all text in this app is not (and it should not be) longer than line length
        newTextSize = newTextSize - 1;
        characters = stringLength * 5 * newTextSize + stringLength * newTextSize;
    }
    tft.setTextSize(newTextSize);

    if ((stringLength > 16) && (newTextSize > 2))
    {
        // there is an error with text that is 17 characters long with size of 2
        // so this IF statement is explicitly for that error, to make text size smaller
        newTextSize = newTextSize - 1;
        characters = stringLength * 5 * newTextSize + stringLength * newTextSize;
    }
    tft.setTextSize(newTextSize);

    if (characters + 10 < lineLength)
    {
        if (textAllign == 1)
        { // left
            tft.setCursor(x, y);
            tft.println(text);
        }
        else if (textAllign == 2)
        { // centered
            if (textSize == 1)
            { // letter length = 5
                real_x = x + int((lineLength - characters) / 2);
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 2)
            { // letter length = 10
                real_x = x + int((lineLength - characters) / 2);
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 3)
            { // letter length = 15
                real_x = x + int((lineLength - characters) / 2);
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 4)
            { // letter length = 20
                real_x = x + int((lineLength - characters) / 2);
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 5)
            { // letter length = 25
                real_x = x + int((lineLength - characters) / 2);
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else
            {
                tft.setTextSize(1);
                tft.setCursor(x, y);
                tft.println("ERROR! Text size is from 1 to 5!");
            }
        }
        else if (textAllign == 3)
        { // right
            if (textSize == 1)
            { // letter length = 5
                real_x = x + lineLength - characters;
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 2)
            { // letter length = 10
                real_x = x + lineLength - characters;
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 3)
            { // letter length = 15
                real_x = x + lineLength - characters;
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 4)
            { // letter length = 20
                real_x = x + lineLength - characters;
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else if (textSize == 5)
            { // letter lenght = 25
                real_x = x + lineLength - characters;
                tft.setCursor(real_x, y);
                tft.println(text);
            }
            else
            {
                tft.setTextSize(1);
                tft.setCursor(x, y);
                tft.println("ERROR! Text size is from 1 to 5!");
            }
        }
        else
        {
            tft.setTextSize(1);
            tft.setCursor(x, y);
            tft.println("ERROR! TextAlign is 0, 1 and 2!");
        }
    }
    else
    {
        tft.setCursor(x, y);
        tft.println(text);
    }
}

void initDisplay()
{
    tft.init();
    tft.setRotation(3);

    uint16_t calData[5] = {251, 3545, 202, 3420, 2};
    tft.setTouch(calData);

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);

    printText(0, 120, "Domopool startup !", 2, 2);
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
}

void pageOTA()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    printText(0, 120, "Firmware update ...", 2, 2);
}
void displayTemp(Config &config)
{
    printText(0, 10, "Temperature", 1, 1);

    String text = "-> Ambiant temperature: ";
    text += config.metrics.curTempAmbiant;
    text += " °C";
    printText(0, 20, text, 1, 1);

    text = "-> Water temperature: ";
    text += config.metrics.curTempWater;
    text += " °C";
    printText(0, 30, text, 1, 1);
}
void displayPump(Config &config)
{
    printText(0, 50, "Pump:", 1, 1);
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

    button[0].initButton(&tft, 80, 180, 158, 120, TFT_BLACK, color, TFT_BLACK, "Filter", 2);
    button[0].drawButton();

    tft.fillRoundRect(162, 120, 158, 120, 5, TFT_BLUE);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}
void page1(Config &config)
{
    tft.fillScreen(TFT_BLACK);
    displayTemp(config);
    displayPump(config);
}
