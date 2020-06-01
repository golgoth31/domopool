#include "Anetwork.h"

WebServer server(80);

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
    if (!MDNS.begin("domopool"))
    { //http://esp32.local
        Serial.println("Error setting up MDNS responder!");
        while (1)
        {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
    /*return index page which is stored in serverIndex */
    StaticJsonDocument<ConfigDocSize> httpResponse;
    httpResponse["version"] = "test";
    String compile = __DATE__;
    compile += " ";
    compile += __TIME__;
    httpResponse["compile"] = compile;
    String output;
    serializeJson(httpResponse, output);
    server.on("/", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "application/json", &output);
    });
    server.on("/serverIndex", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", serverIndex);
    });
    /*handling uploading firmware file */
    server.on(
        "/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart(); }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    } });
    server.begin();

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
    //                     convert2doc(config, httpResponse);
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
