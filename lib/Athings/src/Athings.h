#define LARGE_JSON_BUFFERS 1

#include <Arduino.h>
#include <Thing.h>
#include <ESPWebThingAdapter.h>

#include <Aconfig.h>

#ifndef __ATHINGS_H_UNDEFINED__
#define __ATHINGS_H_UNDEFINED__
void initWebThings();
void handleWebThings(Config &config);
#endif
