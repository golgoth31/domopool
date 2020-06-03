#include "Athings.h"

WebThingAdapter *adapter;
const char *pumpTypes[] = {"OnOffSwitch", "BinarySensor", nullptr};

ThingDevice wfilterPump("filterPump", "Filter pump", pumpTypes);
ThingProperty wfilterOn("state", "Filter pump state", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);
ThingProperty wfilterForce("force", "Force filter pump", ThingDataType::BOOLEAN, "OnOffProperty", nullptr);
ThingProperty wfilterAlarm("alarm", "Filter pump alarm", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);

ThingDevice wphPump("phPump", "ph pump", pumpTypes);
ThingProperty wphOn("state", "ph pump state", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);
ThingProperty wphForce("force", "Force ph pump", ThingDataType::BOOLEAN, "OnOffProperty", nullptr);
ThingProperty wphAlarm("alarm", "ph pump alarm", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);

void initWebThings()
{
    // wfilterPump.id = "filterPump";
    // wfilterPump.description = "Filter pump";
    // wfilterPump.type = pumpTypes;

    // wfilterOn.id = "on";
    // wfilterOn.description = "Filter pump state";
    // wfilterOn.type = ThingDataType::BOOLEAN;
    // wfilterOn.atType = "BooleanProperty";

    // wfilterForce.id = "force";
    // wfilterForce.description = "Force filter pump";
    // wfilterForce.type = ThingDataType::BOOLEAN;
    // wfilterForce.atType = "OnOffProperty";

    // wfilterAlarm.id = "alarm";
    // wfilterAlarm.description = "Filter pump alarm";
    // wfilterAlarm.type = ThingDataType::BOOLEAN;
    // wfilterAlarm.atType = "BooleanProperty";

    // ThingDevice wphPump("phPump", "ph pump", pumpTypes);
    // ThingProperty wphOn("on", "ph pump state", ThingDataType::BOOLEAN, "BooleanProperty");
    // ThingProperty wphForce("force", "Force ph pump", ThingDataType::BOOLEAN, "OnOffProperty");
    // ThingProperty wphAlarm("alarm", "ph pump alarm", ThingDataType::BOOLEAN, "BooleanProperty");

    adapter = new WebThingAdapter("domopool", WiFi.localIP(), 8080);

    wfilterPump.addProperty(&wfilterOn);
    wfilterPump.addProperty(&wfilterForce);
    wfilterPump.addProperty(&wfilterAlarm);
    adapter->addDevice(&wfilterPump);

    wphPump.addProperty(&wphOn);
    wphPump.addProperty(&wphForce);
    wphPump.addProperty(&wphAlarm);
    adapter->addDevice(&wphPump);

    adapter->begin();
}

void handleWebThings()
{
    adapter->update();
};
