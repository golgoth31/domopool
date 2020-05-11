#include <Alarms.h>

#include <LiquidCrystal.h>
#include <SD.h>

#ifndef __AMEGA2560_H__
#define __AMEGA2560_H__
bool initStorage();
File openFile(const char *filename);
File saveFile(const char *filename);
#endif
