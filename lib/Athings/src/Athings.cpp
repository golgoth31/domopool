#include "Athings.h"

WebThingAdapter *adapter;

void initWebThings()
{
    adapter = new WebThingAdapter("domopool", WiFi.localIP(), 8080);
}
