#include "Anetwork.h"

WiFiClient espClient;
PubSubClient client(espClient);
AsyncWebServer server(80);
IPAddress MQTTServer;
const char *localIP;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int ip1, ip2, ip3, ip4;
int8_t OTAdot = 0;
const int ConfigDocSize = 2048;

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("[MQTT] Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void handleUploadUi(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        Serial.println("UploadStart: " + filename);
        // open the file on first call and store the file handle in the request object
        request->_tempFile = SPIFFS.open("/" + filename, "w");
    }
    if (len)
    {
        // stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final)
    {
        Serial.println("UploadEnd: " + filename + ",size: " + index + len);
        // close the file handle as the upload is now done
        request->_tempFile.close();
        request->redirect("/ui");
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.println("[MQTT] Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "domopool-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("[MQTT] connected");
            // // Once connected, publish an announcement...
            // client.publish("metrics", "hello world");
            // // ... and resubscribe
            // client.subscribe("inTopic");
        }
        else
        {
            Serial.print("[MQTT] failed, rc=");
            Serial.print(client.state());
            Serial.println("[MQTT] try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void software_Reboot()
{
    delay(1000);
    esp_restart();
}

bool checkIP(const char *ip)
{
    IPAddress checkIp;
    if (checkIp.fromString(ip))
    {
        sscanf(ip, "%u.%u.%u.%u", &ip1, &ip2, &ip3, &ip4);
        return true;
    }
    return false;
}

void startOTA()
{
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setHostname("domopool");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
    ArduinoOTA.setMdnsEnabled(true);
    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "Firmware";
            else // U_SPIFFS
                type = "SPIFFS";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            displayProgressBarText(type + " update", TFT_RED);
            server.end();
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
            OTAdot = 0;
            // server.begin();
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            int percent = progress / (total / 100);
            Serial.printf("Progress: %u%%\r", percent);
            displayProgressBar(percent, TFT_RED);
            OTAdot++;
            if (OTAdot > 5)
            {
                OTAdot = 0;
            }
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.begin();
}

void startServer(Config &config)
{
    // For CORS
    server.on("/*", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        request->send(200);
    });

    // Serving pages
    // root
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<ConfigDocSize> httpResponse;
        String compile = __DATE__;
        compile += " ";
        compile += __TIME__;
        httpResponse["compile"] = compile;
        httpResponse["board_name"] = ARDUINO_BOARD;
        httpResponse["versions"]["domopool"] = "test";
        httpResponse["versions"]["platformio"] = PLATFORMIO;
        httpResponse["versions"]["esp_idf"] = esp_get_idf_version();
        httpResponse["versions"]["xtensa"] = __VERSION__;
        httpResponse["versions"]["arduinojson"] = ARDUINOJSON_VERSION;
        httpResponse["versions"]["tft_espi"] = TFT_ESPI_VERSION;
        String output;
        serializeJsonPretty(httpResponse, output);
        request->send(200, "application/json", output);
    });

    // config
    server.on("/config", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        DynamicJsonDocument httpResponse(ConfigDocSize);
        config2doc(config, httpResponse);
        String output = "";
        serializeJsonPretty(httpResponse, output);
        request->send(200, "application/json", output);
    });
    // AsyncCallbackJsonWebHandler *configHandler = new AsyncCallbackJsonWebHandler("/config", [&config](AsyncWebServerRequest *request, JsonVariant &json) {
    //     JsonObject jsonObj = json.as<JsonObject>();
    //     saveJson(jsonObj, config, "/config.jsn");
    //     request->send(200);
    // });
    // server.addHandler(configHandler);

    // metrics
    server.on("/metrics", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        DynamicJsonDocument httpResponse(ConfigDocSize);
        metrics2doc(config, httpResponse);
        String output = "";
        serializeJsonPretty(httpResponse, output);
        request->send(200, "application/json", output);
    });

    // states
    server.on("/states", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        DynamicJsonDocument httpResponse(ConfigDocSize);
        states2doc(config, httpResponse);
        String output = "";
        serializeJsonPretty(httpResponse, output);
        request->send(200, "application/json", output);
    });

    // healthz
    server.on("/healthz", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200);
    });

    // metrics
    server.on("/readyz", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        if (config.states.startup)
        {
            request->send(404);
        }
        request->send(200);
    });

    // favicon
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.png", "image/png");
    });

    // ui
    server.on("/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/ui/bundle.js");
    });
    server.serveStatic("/ui/", SPIFFS, "/").setDefaultFile("index.html");
    server.on(
        "/ui/upload", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK");
            response->addHeader("Connection", "close");
            request->send(response);
        },
        [](AsyncWebServerRequest *request,
           const String &filename,
           size_t index,
           uint8_t *data,
           size_t len, bool final) { handleUploadUi(request, filename, index, data, len, final); });

    // api
    // filter
    server.on("/api/v1/filter", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        StaticJsonDocument<ConfigDocSize> httpResponse;
        String state;
        if (config.states.automatic)
        {
            state = "auto";
        }
        else if (config.pump.forceFilter)
        {
            state = "start";
        }
        else
        {
            state = "stop";
        }
        httpResponse["state"] = state;
        httpResponse["duration"] = config.pump.forceDuration;
        String output;
        serializeJsonPretty(httpResponse, output);
        request->send(200, "application/json", output);
    });
    AsyncCallbackJsonWebHandler *filterHandler = new AsyncCallbackJsonWebHandler(
        "/api/v1/filter",
        [&config](AsyncWebServerRequest *request, JsonVariant &json) {
            JsonObject jsonObj = json.as<JsonObject>();
            if (jsonObj["state"] == "force")
            {
                startPump(1);
            }
            else if (jsonObj["state"] == "stop")
            {
                stopPump(1);
            }
            else if (jsonObj["state"] == "auto")
            {
                setPumpAuto();
            }
            else
            {
                request->send(500);
            }
            int duration = jsonObj["duration"];
            if (duration)
            {
                config.pump.forceDuration = duration;
            }
            request->send(200, "application/json", "{}");
        });
    server.addHandler(filterHandler);

    // mqtt
    AsyncCallbackJsonWebHandler *mqttHandler = new AsyncCallbackJsonWebHandler("/api/v1/mqtt", [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject jsonObj = json.as<JsonObject>();
        if (jsonObj["state"] == "start")
        {
            startMqtt();
        }
        else if (jsonObj["state"] == "stop")
        {
            stopMqtt();
        }
        else
        {
            request->send(500);
        }
        request->send(200, "application/json", "{}");
    });
    server.addHandler(mqttHandler);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET,OPTIONS,POST");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "access-control-allow-origin,content-type");
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404);
    });
    server.begin();
}
bool startNetwork(const char *ssid, const char *password, Config &config)
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("[FS] error on opening SPIFFS");
    };
    Serial.println(F("[WiFi] Connecting"));
    Serial.print(F("[WiFi] "));
    bool wifiUp = false;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        Serial.print(".");

        wifiUp = true;
    }

    Serial.println("");
    Serial.println(F("[WiFi] Connected"));
    Serial.print(F("[WiFi] IP address: "));
    Serial.println(WiFi.localIP());

    config.network.ip = WiFi.localIP().toString();

    startServer(config);

    startOTA();

    Serial.println("[WiFi] Ready");
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    client.setServer(config.network.mqtt.server.c_str(), 1883);
    client.setCallback(callback);

    return wifiUp;
}

void stopNetwork()
{
    ArduinoOTA.end();
    server.end();
    client.disconnect();
}
void restartNetwork(const char *ssid, const char *password, Config &config)
{
    if (WiFi.status() == WL_CONNECTION_LOST)
    {
        stopNetwork();
        startNetwork(ssid, password, config);
    }
}
void sendMetricsMqtt(Config &config)
{
    DynamicJsonDocument doc(ConfigDocSize);
    metrics2doc(config, doc);
    String output = "";
    serializeJson(doc, output);
    client.publish("domopool/metrics", output.c_str());
}
void sendStatesMqtt(Config &config)
{
    DynamicJsonDocument doc(ConfigDocSize);
    states2doc(config, doc);
    String output = "";
    serializeJson(doc, output);
    client.publish("domopool/states", output.c_str());
}

void sendData(Config &config)
{
    ArduinoOTA.handle();
    if (config.network.mqtt.enabled)
    {
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();
    }
    else
    {
        if (client.connected())
        {
            client.disconnect();
        }
    }
}
