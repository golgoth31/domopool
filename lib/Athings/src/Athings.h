#include <Thing.h>
#include <ESPWebThingAdapter.h>

#ifndef __ATHINGS_H_UNDEFINED__
#define __ATHINGS_H_UNDEFINED__
const char *pumpTypes[] = {"OnOffSwitch", "BinarySensor", nullptr};
ThingDevice filterPump("filterPump", "Filter pump", pumpTypes);
ThingProperty filterOn("on", "Filter pump state", ThingDataType::BOOLEAN, "BooleanProperty");
ThingProperty filterForce("force", "Force filter pump", ThingDataType::BOOLEAN, "OnOffProperty");
ThingProperty filterAlarm("alarm", "Filter pump alarm", ThingDataType::BOOLEAN, "BooleanProperty");
#endif
