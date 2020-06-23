#include "Anetwork.h"

AsyncWebServer server(80);
IPAddress MQTTServer;
const char *localIP;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int ip1, ip2, ip3, ip4;
int8_t OTAdot = 0;
const int ConfigDocSize = 2048;

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

bool startNetwork(const char *ssid, const char *password, Config &config)
{
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

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<ConfigDocSize> httpResponse;
        httpResponse["version"] = "test";
        String compile = __DATE__;
        compile += " ";
        compile += __TIME__;
        httpResponse["compile"] = compile;
        String output;
        serializeJsonPretty(httpResponse, output);
        request->send(200, "application/json", output);
    });
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
    server.on("/metrics", HTTP_GET, [&config](AsyncWebServerRequest *request) {
        DynamicJsonDocument httpResponse(ConfigDocSize);
        metrics2doc(config, httpResponse);
        String output = "";
        serializeJsonPretty(httpResponse, output);
        request->send(200, "application/json", output);
    });
    server.on("/stop-filter", HTTP_POST, [](AsyncWebServerRequest *request) {
        stopPump(1);
        request->send(200, "application/json", "{}");
    });
    server.on("/start-filter", HTTP_POST, [](AsyncWebServerRequest *request) {
        startPump(1);
        request->send(200, "application/json", "{}");
    });
    server.on("/set-pump-auto", HTTP_POST, [](AsyncWebServerRequest *request) {
        setPumpAuto();
        request->send(200, "application/json", "{}");
    });
    server.begin();

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
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            pageOTA();
            server.end();
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
            OTAdot = 0;
            server.begin();
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            pageOTAdot(OTAdot);
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

    Serial.println("[WiFi] Ready");
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    return wifiUp;
}

void sendData(Config &config)
{
    ArduinoOTA.handle();

    // WiFiClient client = server.available();
    // bool postRequest = false;
    // if (client)
    // {

    //     Serial.println(F("[Ethernet] New client request"));

    //     boolean currentLineIsBlank = true;
    //     while (client.connected())
    //     {
    //         if (client.available())
    //         {
    //             String reqType = client.readStringUntil(' ');
    //             String reqURI = client.readStringUntil(' ');
    //             String reqProtocol = client.readStringUntil('\n');

    //             // String reqHeaders = client.readStringUntil('\n');
    //             String reqHeaders;
    //             while (currentLineIsBlank)
    //             {
    //                 reqHeaders = client.readStringUntil('\n');
    //                 if (reqHeaders.startsWith("\r"))
    //                 {
    //                     currentLineIsBlank = false;
    //                 }
    //             }

    //             String reqBody = client.readStringUntil('\n');

    //             // Serial.println(reqType);
    //             // Serial.println(reqURI);
    //             // Serial.println(reqProtocol);
    //             // Serial.println(reqBody);
    //             StaticJsonDocument<ConfigDocSize> httpResponse;
    //             if (reqType.equals("GET"))
    //             {
    //                 if (reqURI.equals("/"))
    //                 {
    //                     httpResponse["version"] = "test";
    //                     String compile = __DATE__;
    //                     compile += " ";
    //                     compile += __TIME__;
    //                     httpResponse["compile"] = compile;

    //                     Serial.println(compile);

    //                     client.print(F("Content-Length: "));
    //                     client.println(measureJsonPretty(httpResponse));
    //                     client.println();

    //                     serializeJsonPretty(httpResponse, client);
    //                 }
    //                 else if (reqURI.equals("/config"))
    //                 {
    //                     config2doc(config, httpResponse);
    //                     response200(client);
    //                     client.print(F("Content-Length: "));
    //                     client.println(measureJsonPretty(httpResponse));
    //                     client.println();

    //                     serializeJsonPretty(httpResponse, client);
    //                 }
    //                 else if (reqURI.equals("/metrics"))
    //                 {
    //                     metrics2doc(config, httpResponse);
    //                     response200(client);
    //                     client.print(F("Content-Length: "));
    //                     client.println(measureJsonPretty(httpResponse));
    //                     client.println();

    //                     serializeJsonPretty(httpResponse, client);
    //                 }
    //                 else
    //                 {
    //                     response404(client);
    //                 }
    //             }
    //             else if (reqType.equals("POST") && config.network.allowPost)
    //             {
    //                 if (reqURI.equals("/reboot"))
    //                 {
    //                     response200(client);
    //                     postRequest = true;
    //                 }
    //                 else if (reqURI.equals("/reset"))
    //                 {
    //                     resetEepromSensorsTemp();
    //                     response200(client);
    //                     postRequest = true;
    //                 }
    //                 else if (reqURI.equals("/config"))
    //                 {

    //                     Serial.print(F("[Eth] Post body: "));
    //                     Serial.println(reqBody);

    //                     bool saved = saveJson(reqBody, config, "config.jsn");
    //                     if (saved)
    //                     {
    //                         response200(client);
    //                     }
    //                     else
    //                     {
    //                         response500(client);
    //                     }
    //                 }
    //                 else
    //                 {
    //                     response404(client);
    //                 }
    //             }
    //             client.println();
    //         }
    //         // give the web browser time to receive the data
    //         delay(1);
    //         // close the connection:
    //         client.stop();

    //         Serial.println(F("[Eth] Response sent"));

    //         if (postRequest)
    //         {

    //             Serial.println(F("[Eth] Rebooting"));

    //             software_Reboot();
    //         }
    //     }
    // }
}
