#include "Anetwork.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);
AsyncWebServer server(80);
IPAddress MQTTServer;
const char *localIP;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int ip1, ip2, ip3, ip4;
int8_t OTAdot = 0;
const int ConfigDocSize = 2048;
bool wifiEnabled = true;

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
        request->send(200);
    }
}

void handleBodyRelay(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
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
    domopool_Relay locswitch = domopool_Relay_init_default;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, total);
    /* Now we are ready to decode the message. */
    bool status = pb_decode(&stream, domopool_Relay_fields, &locswitch);

    /* Check for errors... */
    if (!status)
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        request->send(500);
    }

    switch (locswitch.state)
    {
    case domopool_Relay_states_start:
        startRelay(locswitch.relay, locswitch.duration);
        break;
    case domopool_Relay_states_stop:
        stopRelay(locswitch.relay);
        break;

    default:
        break;
    }
}

void handleBodyMqtt(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
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
    domopool_Mqtt mqtt = domopool_Mqtt_init_default;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, total);
    /* Now we are ready to decode the message. */
    bool status = pb_decode(&stream, domopool_Mqtt_fields, &mqtt);

    /* Check for errors... */
    if (!status)
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }
    setMqtt(mqtt.server);
}

void handleBodyAnalogSensor(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
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
    domopool_AnalogSensor sens = domopool_AnalogSensor_init_default;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, total);
    /* Now we are ready to decode the message. */
    bool status = pb_decode(&stream, domopool_AnalogSensor_fields, &sens);

    /* Check for errors... */
    if (!status)
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }
    setWP(sens.adc_pin, sens.threshold, sens.threshold_accuracy, sens.vmin, sens.vmax, sens.auto_cal);
}

void handleBodyADC(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
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
    domopool_Sensors sens = domopool_Sensors_init_default;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, total);
    /* Now we are ready to decode the message. */
    bool status = pb_decode(&stream, domopool_Sensors_fields, &sens);

    /* Check for errors... */
    if (!status)
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }
    setADC(sens.adc_mode, sens.adc_gain, sens.adc_datarate);
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
    server.on(
        "/",
        HTTP_GET,
        [](AsyncWebServerRequest *request) {
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
            strcpy(infos.versions.dallastemp, DALLASTEMPLIBVERSION);
            strcpy(infos.versions.ads1115, ADS1X15_LIB_VERSION);
            strcpy(infos.versions.nanopb, "0.4.4");
            strcpy(infos.versions.mqtt, "3.1.1");
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

    // healthz
    server.on(
        "/healthz",
        HTTP_GET,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        });

    // metrics
    server.on(
        "/readyz",
        HTTP_GET,
        [&config](AsyncWebServerRequest *request) {
            if (config.states.startup)
            {
                request->send(404);
            }
            request->send(200);
        });

    // favicon
    server.on(
        "/favicon.*",
        HTTP_GET,
        [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/favicon.png", "image/png");
        });

    // ui
    server.on(
        "/bundle.js",
        HTTP_GET,
        [](AsyncWebServerRequest *request) {
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
           size_t len, bool final) {
            handleUploadUi(request, filename, index, data, len, final);
        });

    // api
    // relay
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
           size_t total) {
            handleBodyRelay(request, data, len, index, total);
        });
    server.on(
        "/api/v1/ch",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
            handleBodyRelay(request, data, len, index, total);
        });
    server.on(
        "/api/v1/ph",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
            handleBodyRelay(request, data, len, index, total);
        });
    server.on(
        "/api/v1/light",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
            handleBodyRelay(request, data, len, index, total);
        });
    server.on(
        "/api/v1/auto",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            setRelayAuto();
            request->send(200);
        });

    // mqtt
    server.on(
        "/api/v1/mqtt/set",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
            handleBodyMqtt(request, data, len, index, total);
        });
    server.on(
        "/api/v1/mqtt/enable",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            startMqtt();
            request->send(200);
        });
    server.on(
        "/api/v1/mqtt/disable",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            stopMqtt();
            request->send(200);
        });

    // sensors
    server.on(
        "/api/v1/sensors/reset",
        HTTP_POST,
        [&config](AsyncWebServerRequest *request) {
            resetSensorsTempAddr(config);
            saveConfiguration(config);
            request->send(200);
        });

    // alarms
    server.on(
        "/api/v1/alarms/reset",
        HTTP_POST,
        [&config](AsyncWebServerRequest *request) {
            config.alarms = domopool_Alarms_init_default;
            config.alarms.has_ads1115 = true;
            config.alarms.has_mqtt = true;
            config.alarms.mqtt = domopool_MqttAlarms_init_default;
            config.alarms.ads1115 = domopool_Ads115Alarms_init_default;
            request->send(200);
        });

    // water pressure
    server.on(
        "/api/v1/wp/set",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
            handleBodyAnalogSensor(request, data, len, index, total);
        });
    server.on(
        "/api/v1/wp/enable",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            enableWP();
            request->send(200);
        });
    server.on(
        "/api/v1/wp/disable",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            disableWP();
            request->send(200);
        });

    // water pressure
    server.on(
        "/api/v1/adc/set",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200);
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
            handleBodyADC(request, data, len, index, total);
        });

    // reboot
    server.on(
        "/api/v1/reboot",
        HTTP_POST,
        [&config](AsyncWebServerRequest *request) {
            saveConfiguration(config);
            reboot();
            request->send(200);
        });

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
    server.on(
        "/api/v1/config/reset",
        HTTP_POST,
        [&config](AsyncWebServerRequest *request) {
            resetConfig();
            request->send(200);
            reboot();
        });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET,OPTIONS,POST");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "access-control-allow-origin,content-type");
    server.onNotFound(
        [](AsyncWebServerRequest *request) {
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
    Serial.print(F("[WiFi] Connecting "));
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() == WL_CONNECTED)
    {
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

        Serial.println("[WiFi] Ready");

        mqttClient.setServer(config.network.mqtt.server, 1883);
        mqttClient.setCallback(callback);
    }
    return WiFi.isConnected();
}

void stopNetwork()
{
    ArduinoOTA.end();
    server.end();
    mqttClient.disconnect();
    WiFi.disconnect();
}

void restartNetwork(const char *ssid, const char *password, domopool_Config &config)
{
    stopNetwork();
    delay(1000);
    config.states.net_active = startNetwork(ssid, password, config);
}

void sendMetricsMqtt(domopool_Config &config)
{
    config.metrics.time = getCurrentTime();
    DynamicJsonDocument doc(ConfigDocSize);
    metrics2doc(config, doc);
    String output = "";
    serializeJson(doc, output);
    if (!mqttClient.publish("domopool/metrics", output.c_str()))
    {
        config.alarms.mqtt.metrics = true;
    }
    // else
    // {
    //     config.alarms.mqtt.metrics = false;
    // }
}

void sendStatesMqtt(domopool_Config &config)
{
    DynamicJsonDocument doc(ConfigDocSize);
    states2doc(config, doc);
    String output = "";
    serializeJson(doc, output);
    if (!mqttClient.publish("domopool/states", output.c_str()))
    {
        config.alarms.mqtt.states = true;
    }
    // else
    // {
    //     config.alarms.mqtt.states = false;
    // }
}

void sendAlarmsMqtt(domopool_Config &config)
{
    DynamicJsonDocument doc(ConfigDocSize);
    alarms2doc(config, doc);
    String output = "";
    serializeJson(doc, output);
    if (!mqttClient.publish("domopool/alarms", output.c_str()))
    {
        config.alarms.mqtt.alarms = true;
    }
    // else
    // {
    //     config.alarms.mqtt.alarms = false;
    // }
}

void reconnect()
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
    }
}

void handleNetwork(domopool_Config &config)
{
    if (WiFi.isConnected())
    {
        ArduinoOTA.handle();
        if (config.network.mqtt.enabled)
        {
            if (!mqttClient.connected())
            {
                reconnect();
            }
            // mqttClient.loop();
        }
        else
        {
            if (mqttClient.connected())
            {
                mqttClient.disconnect();
            }
        }
        config.states.mqtt_connected = mqttClient.connected();
    }
    else
    {
        stopNetwork();
        config.states.net_active = false;
    }
}
