#include "Athings.h"

WebThingAdapter *adapter;
const char *pumpTypes[] = {"OnOffSwitch", "BinarySensor", nullptr};
const char *tempTypes[] = {"TemperatureSensor", nullptr};

ThingDevice wfilterPump("filterPump", "Filter pump", pumpTypes);
ThingProperty wfilterState("state", "Filter pump state", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);
ThingProperty wfilterForce("force", "Force filter pump", ThingDataType::BOOLEAN, "OnOffProperty", nullptr);
ThingProperty wfilterAlarm("alarm", "Filter pump alarm", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);

ThingDevice wphPump("phPump", "ph pump", pumpTypes);
ThingProperty wphState("state", "ph pump state", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);
ThingProperty wphForce("force", "Force ph pump", ThingDataType::BOOLEAN, "OnOffProperty", nullptr);
ThingProperty wphAlarm("alarm", "ph pump alarm", ThingDataType::BOOLEAN, "BooleanProperty", nullptr);

ThingDevice wtemp("temp", "Temperatures", tempTypes);
ThingProperty wtAmd("tamb", "Ambiant temperature", ThingDataType::NUMBER, "TemperatureProperty", nullptr);
ThingProperty wtWater("twater", "Water temperature", ThingDataType::NUMBER, "TemperatureProperty", nullptr);

void initWebThings()
{
    adapter = new WebThingAdapter("domopool", WiFi.localIP(), 8080);

    wfilterPump.addProperty(&wfilterState);
    wfilterPump.addProperty(&wfilterForce);
    wfilterPump.addProperty(&wfilterAlarm);
    adapter->addDevice(&wfilterPump);

    wphPump.addProperty(&wphState);
    wphPump.addProperty(&wphForce);
    wphPump.addProperty(&wphAlarm);
    adapter->addDevice(&wphPump);

    wtemp.addProperty(&wtAmd);
    wtemp.addProperty(&wtWater);
    adapter->addDevice(&wtemp);

    adapter->begin();
}

void handleWebThings(Config &config)
{
    ThingPropertyValue propVal = {.boolean = config.metrics.filterOn};
    wfilterState.setValue(propVal);
    propVal = {.boolean = config.pump.forceFilter};
    wfilterForce.setValue(propVal);
    propVal = {.boolean = config.metrics.alarms.filter};
    wfilterAlarm.setValue(propVal);

    propVal = {.number = config.metrics.curTempAmbiant};
    wtAmd.setValue(propVal);
    propVal = {.number = config.metrics.curTempWater};
    wtWater.setValue(propVal);

    adapter->update();
};
