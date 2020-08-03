#include "Anetwork.h"

WiFiClient espClient;
WiFiServer espServer(1234);
PubSubClient mqttClient(espClient);
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

void handleBodyFilter(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    uint8_t buffer[total];
    if (!index)
    {
        Serial.printf("BodyStart: %u B\n", total);
    }
    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = data[i];
    }
    if (index + len == total)
    {
        Serial.printf("BodyEnd: %u B\n", total);
    }
    /* Allocate space for the decoded message. */
    domopool_Filter filter = domopool_Filter_init_zero;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, total);
    /* Now we are ready to decode the message. */
    bool status = pb_decode(&stream, domopool_Filter_fields, &filter);

    /* Check for errors... */
    if (!status)
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }
    if (filter.state == domopool_Filter_states_start)
    {
        setPumpDuration(filter.duration);
        startPump(1);
    }
    else if (filter.state == domopool_Filter_states_stop)
    {
        stopPump(1);
    }
    else if (filter.state == domopool_Filter_states_auto)
    {
        setPumpAuto();
    }
    else
    {
        request->send(500);
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!mqttClient.connected())
    {
        Serial.println("[MQTT] Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "domopool-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (mqttClient.connect(clientId.c_str()))
        {
            Serial.println("[MQTT] connected");
            // // Once connected, publish an announcement...
            // mqttClient.publish("metrics", "hello world");
            // // ... and resubscribe
            // mqttClient.subscribe("inTopic");
        }
        else
        {
            Serial.print("[MQTT] failed, rc=");
            Serial.print(mqttClient.state());
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

void startServer(domopool_Config &config)
{
    // For CORS
    server.on("/*", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        request->send(200);
    });

    // Serving pages
    // root
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String compile = __DATE__;
        compile += " ";
        compile += __TIME__;
        domopool_Infos infos = domopool_Infos_init_default;
        strcpy(infos.board_name, ARDUINO_BOARD);
        strcpy(infos.compile, compile.c_str());
        infos.has_versions = true;
        strcpy(infos.versions.domopool, "test");
        strcpy(infos.versions.esp_idf, esp_get_idf_version());
        infos.versions.platformio = PLATFORMIO;
        strcpy(infos.versions.tft_espi, TFT_ESPI_VERSION);
        strcpy(infos.versions.xtensa, __VERSION__);
        uint8_t buffer[128];
        size_t message_length;
        bool status;
        pb_ostream_t pb_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        status = pb_encode(&pb_stream, domopool_Infos_fields, &infos);
        message_length = pb_stream.bytes_written;
        if (status)
        {
            AsyncResponseStream *response = request->beginResponseStream("text/plain");
            response->write(buffer, message_length);
            request->send(response);
        }
        else
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&pb_stream));
            request->send(500);
        }
    });

    // metrics
    server.on("/metrics", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        uint8_t buffer[128];
        size_t message_length;
        bool status;
        pb_ostream_t pb_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        status = pb_encode(&pb_stream, domopool_Metrics_fields, &config.metrics);
        message_length = pb_stream.bytes_written;
        if (status)
        {
            AsyncResponseStream *response = request->beginResponseStream("text/plain");
            response->write(buffer, message_length);
            request->send(response);
        }
        else
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&pb_stream));
            request->send(500);
        }
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
    server.on("/favicon.*", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.png", "image/png");
    });

    // ui
    server.on("/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/ui/bundle.js");
    });
    server.serveStatic("/ui/", SPIFFS, "/").setDefaultFile("index.html");
    server.on(
        "/ui/upload",
        HTTP_POST,
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
    // states
    server.on("/api/v1/states", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        uint8_t buffer[128];
        size_t message_length;
        bool status;
        pb_ostream_t pb_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        status = pb_encode(&pb_stream, domopool_States_fields, &config.states);
        message_length = pb_stream.bytes_written;
        if (status)
        {
            AsyncResponseStream *response = request->beginResponseStream("text/plain");
            response->write(buffer, message_length);
            request->send(response);
        }
        else
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&pb_stream));
            request->send(500);
        }
    });

    // filter
    server.on("/api/v1/filter", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        domopool_Filter_states state;
        if (config.states.automatic)
        {
            state = domopool_Filter_states_auto;
        }
        else if (config.pump.force_filter)
        {
            state = domopool_Filter_states_start;
        }
        else
        {
            state = domopool_Filter_states_stop;
        }
        domopool_Filter filter;
        filter.state = state;
        filter.duration = config.pump.force_duration;
        uint8_t buffer[128];
        size_t message_length;
        bool status;
        pb_ostream_t pb_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        status = pb_encode(&pb_stream, domopool_Filter_fields, &filter);
        message_length = pb_stream.bytes_written;
        if (status)
        {
            AsyncResponseStream *response = request->beginResponseStream("text/plain");
            response->write(buffer, message_length);
            request->send(response);
        }
        else
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&pb_stream));
            request->send(500);
        }
    });
    server.on(
        "/api/v1/filter",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) { handleBodyFilter(request, data, len, index, total); });

    // config
    server.on(
        "/api/v1/config",
        HTTP_GET,
        [&config](AsyncWebServerRequest *request) {
            uint8_t buffer[1024];
            size_t message_length;
            bool status;
            pb_ostream_t pb_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
            status = pb_encode(&pb_stream, domopool_Config_fields, &config);
            message_length = pb_stream.bytes_written;
            if (status)
            {
                AsyncResponseStream *response = request->beginResponseStream("text/plain");
                response->write(buffer, message_length);
                request->send(response);
            }
            else
            {
                printf("Encoding failed: %s\n", PB_GET_ERROR(&pb_stream));
                request->send(500);
            }
        });
    // AsyncCallbackJsonWebHandler *configHandler = new AsyncCallbackJsonWebHandler(
    //     "/api/v1/config",
    //     [&config](AsyncWebServerRequest *request, JsonVariant &json) {
    //         JsonObject jsonObj = json.as<JsonObject>();
    //         if (jsonObj["reset"] == "true")
    //         {
    //             resetConfig();
    //         }
    //         else
    //         {
    //             request->send(500);
    //         }
    //         request->send(200, "application/json", "{}");
    //     });
    // server.addHandler(configHandler);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET,OPTIONS,POST");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "access-control-allow-origin,content-type");
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404);
    });
    server.begin();
}
bool startNetwork(const char *ssid, const char *password, domopool_Config &config)
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

    strcpy(config.network.ip, WiFi.localIP().toString().c_str());
    strcpy(config.network.gateway, WiFi.gatewayIP().toString().c_str());
    strcpy(config.network.dns, WiFi.dnsIP().toString().c_str());
    strcpy(config.network.netmask, WiFi.subnetMask().toString().c_str());

    startServer(config);

    startOTA();
    espServer.begin();

    WiFiClient client = espServer.available();

    Serial.println("[WiFi] Ready");
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    mqttClient.setServer(config.network.mqtt.server, 1883);
    mqttClient.setCallback(callback);

    return wifiUp;
}

void stopNetwork()
{
    ArduinoOTA.end();
    server.end();
    mqttClient.disconnect();
}
void restartNetwork(const char *ssid, const char *password, domopool_Config &config)
{
    if (WiFi.status() == WL_CONNECTION_LOST)
    {
        stopNetwork();
        startNetwork(ssid, password, config);
    }
}
void sendMetricsMqtt(domopool_Config &config)
{
    // DynamicJsonDocument doc(ConfigDocSize);
    // metrics2doc(config, doc);
    // String output = "";
    // serializeJson(doc, output);
    // mqttClient.publish("domopool/metrics", output.c_str());
}
void sendStatesMqtt(domopool_Config &config)
{
    // DynamicJsonDocument doc(ConfigDocSize);
    // states2doc(config, doc);
    // String output = "";
    // serializeJson(doc, output);
    // mqttClient.publish("domopool/states", output.c_str());
}

void sendData(domopool_Config &config)
{
    ArduinoOTA.handle();
    if (config.network.mqtt.enabled)
    {
        if (!mqttClient.connected())
        {
            reconnect();
        }
        mqttClient.loop();
    }
    else
    {
        if (mqttClient.connected())
        {
            mqttClient.disconnect();
        }
    }
}
