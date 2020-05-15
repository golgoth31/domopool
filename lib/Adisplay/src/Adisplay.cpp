#include "Adisplay.h"

byte Degree[8] = {
    0b00000,
    0b00100,
    0b01010,
    0b00100,
    0b00000,
    0b00000,
    0b00000,
    0b00000};

void lcdInit(LiquidCrystal &lcd, int Hpix, int Vpix)
{
    lcd.begin(16, 2);
    lcd.createChar(0, Degree);
    lcd.clear();
    lcd.setCursor(0, 0);
}
void lcdPage1(LiquidCrystal &lcd, Config &config)
{
    lcd.clear();
    // print water temp
    lcd.setCursor(0, 0);
    lcd.print(F("Tw:"));
    lcd.setCursor(3, 0);
    lcd.print(config.data.curTempWater);
    lcd.write(byte(0));
    lcd.print(F("C"));
    // print hour
    lcd.setCursor(11, 0);
    lcd.print(printTime(false));
    // print ambiant temp
    lcd.setCursor(0, 1);
    lcd.print(F("Tamb:"));
    lcd.setCursor(5, 1);
    lcd.print(config.sensors.tamb.val);
    lcd.write(byte(0));
    lcd.print(F("C"));
}
void lcdMenuPage1(LiquidCrystal &lcd, Config &config)
{
    lcd.clear();
    // print water temp
    lcd.setCursor(0, 0);
    lcd.print(F("[Time] Set hour ?"));
}
void lcdMenuPage2(LiquidCrystal &lcd, Config &config)
{
    lcd.clear();
    // print water temp
    lcd.setCursor(0, 0);
    lcd.print(F("[Eth] Allow post?"));
}
