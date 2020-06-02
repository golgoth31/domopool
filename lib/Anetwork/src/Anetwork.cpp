#include "Anetwork.h"

WiFiServer server(80);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int ip1, ip2, ip3, ip4;

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

bool startNetwork(const char *ssid, const char *password)
{
    bool wifiUp = false;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        wifiUp = true;
    }

    Serial.println("");
    Serial.println(F("[WiFi] WiFi connected"));
    Serial.println(F("[WiFi] IP address: "));
    Serial.println(WiFi.localIP());

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
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
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

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return wifiUp;
}

void response200(WiFiClient &client)
{
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));
}
void response404(WiFiClient &client)
{
    client.println(F("HTTP/1.1 404 NotFound"));
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));
}
void response500(WiFiClient &client)
{
    client.println(F("HTTP/1.1 500 Error"));
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));
}

void sendData(Config &config)
{
    ArduinoOTA.handle();

    WiFiClient client = server.available();
    bool postRequest = false;
    if (client)
    {
        Serial.println(F("[Ethernet] New client request"));
        boolean currentLineIsBlank = true;
        while (client.connected())
        {
            if (client.available())
            {
                String reqType = client.readStringUntil(' ');
                String reqURI = client.readStringUntil(' ');
                String reqProtocol = client.readStringUntil('\n');

                // String reqHeaders = client.readStringUntil('\n');
                String reqHeaders;
                while (currentLineIsBlank)
                {
                    reqHeaders = client.readStringUntil('\n');
                    if (reqHeaders.startsWith("\r"))
                    {
                        currentLineIsBlank = false;
                    }
                }

                String reqBody = client.readStringUntil('\n');

                // Serial.println(reqType);
                // Serial.println(reqURI);
                // Serial.println(reqProtocol);
                // Serial.println(reqBody);
                StaticJsonDocument<ConfigDocSize> httpResponse;
                if (reqType.equals("GET"))
                {
                    if (reqURI.equals("/"))
                    {
                        httpResponse["version"] = "test";
                        String compile = __DATE__;
                        compile += " ";
                        compile += __TIME__;
                        httpResponse["compile"] = compile;
                        Serial.println(compile);
                        client.print(F("Content-Length: "));
                        client.println(measureJsonPretty(httpResponse));
                        client.println();

                        serializeJsonPretty(httpResponse, client);
                    }
                    else if (reqURI.equals("/config"))
                    {
                        convert2doc(config, httpResponse);
                        response200(client);
                        client.print(F("Content-Length: "));
                        client.println(measureJsonPretty(httpResponse));
                        client.println();

                        serializeJsonPretty(httpResponse, client);
                    }
                    else
                    {
                        response404(client);
                    }
                }
                else if (reqType.equals("POST") && config.network.allowPost)
                {
                    if (reqURI.equals("/reboot"))
                    {
                        response200(client);
                        postRequest = true;
                    }
                    else if (reqURI.equals("/reset"))
                    {
                        resetEepromSensorsTemp();
                        response200(client);
                        postRequest = true;
                    }
                    else if (reqURI.equals("/config"))
                    {

                        Serial.print(F("[Eth] Post body: "));
                        Serial.println(reqBody);
                        bool saved = saveJson(reqBody, config, "config.jsn");
                        if (saved)
                        {
                            response200(client);
                        }
                        else
                        {
                            response500(client);
                        }
                    }
                    else
                    {
                        response404(client);
                    }
                }
                client.println();
            }
            // give the web browser time to receive the data
            delay(1);
            // close the connection:
            client.stop();
            Serial.println(F("[Eth] Response sent"));
            if (postRequest)
            {
                Serial.println(F("[Eth] Rebooting"));
                software_Reboot();
            }
        }
    }
}
