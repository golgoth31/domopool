#include <Aconfig.h>
#include <Atime.h>

#include <Arduino.h>
#include <LiquidCrystal.h>

#ifndef __ADISPLAY_H__
#define __ADISPLAY_H__
void lcdInit(LiquidCrystal &lcd, int Hpix, int Vpix);
void lcdPage1(LiquidCrystal &lcd, Config &config);
#endif
