#include <LiquidCrystal.h>
#include <SD.h>

#ifndef __AMEGA2560_H__
#define __AMEGA2560_H__
void initStorage();
File openFile(const char *filename);
File saveFile(const char *filename);
#endif
