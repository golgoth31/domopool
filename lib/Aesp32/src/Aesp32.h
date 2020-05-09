#include <LiquidCrystal.h>
#include <FS.h>
#include <SPIFFS.h>

#ifndef __AESP32_H__
#define __AESP32_H__
void initStorage();
File openFile(const char *filename);
File saveFile(const char *filename);
#endif
