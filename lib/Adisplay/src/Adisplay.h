// #define USER_SETUP_LOADED 1
// #define ILI9341_DRIVER 1
// #define TFT_WIDTH 240
// #define TFT_HEIGHT 320
// #define TFT_MISO 19
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS 5
// #define TFT_DC 4
// #define TFT_RST 22
// #define HAVE_TOUCHPAD 1
// #define TOUCH_CS 14
// #define TOUCH_IRQ 2
// #define LOAD_GLCD 1
// #define LOAD_GFXFF 1
// #define SPI_FREQUENCY 27000000
// #define SPI_TOUCH_FREQUENCY 2500000
// #define SPI_READ_FREQUENCY 20000000
// #define TFT_BL 15
// #define TFT_BACKLIGHT_ON LOW

// #define GFXFF 1
// #define GLCD 0
// #define FONT2 2
// #define FF17 &FreeSans9pt7b

#include <SPI.h>
#include <TFT_eSPI.h>
#include <TimeLib.h>

#include <Aconfig.h>
#include <Atime.h>

#ifndef __ADISPLAY_H_UNDEFINED__
#define __ADISPLAY_H_UNDEFINED__
void initDisplay();
void page1(Config &config);
void pageOTA();
void displayPump(Config &config);
void displayTemp(Config &config);
void displayDate();
#endif
