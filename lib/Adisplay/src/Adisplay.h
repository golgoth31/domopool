#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TimeLib.h>

#include <Aconfig.h>
#include <Atime.h>

#ifndef __ADISPLAY_H__
#define __ADISPLAY_H__
void initDisplay();
void displayPageMain(domopool_Config &config);
void displayProgressBarText(String text, uint32_t color);
void displayProgressBar(int percent, uint32_t color);
void displayPageBoot();
void display2boot(String text, domopool_Config &config);
void displayPump(domopool_Config &config);
void displayTemp(domopool_Config &config);
void displayDate(domopool_Config &config);
void displayServices(domopool_Config &config);
void displayPressed(domopool_Config &config);
void displayStartup();
#endif
