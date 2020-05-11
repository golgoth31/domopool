#include "Aethernet.h"

#ifdef __AVR_ATmega2560__
EthernetServer server(80);
#endif

#ifdef ESP32
WiFiServer server(80);
#endif

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int ip1, ip2, ip3, ip4;

void software_Reboot()
{
    wdt_enable(WDTO_1S);

    while (1)
    {
    }
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

bool startNetwork(Network &conf)
{
    int ethOK = 1;
    if (conf.dhcp)
    {
        ethOK = Ethernet.begin(mac);
    }
    else
    {
        if (checkIP(conf.ip))
        {
            IPAddress ip(ip1, ip2, ip3, ip4);
            checkIP(conf.netmask);
            IPAddress netmask(ip1, ip2, ip3, ip4);
            checkIP(conf.gateway);
            IPAddress gateway(ip1, ip2, ip3, ip4);
            checkIP(conf.dns);
            IPAddress dns(ip1, ip2, ip3, ip4);
            Ethernet.begin(mac, ip, dns, gateway, netmask);
        }
        else
        {
            return false;
        }
    }

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :("));
        return false;
    }
    if (Ethernet.linkStatus() == LinkOFF) // not working on W5100 shield
    {
        Serial.println(F("Ethernet cable is not connected."));
        return false;
    }

    // start the server
    if (ethOK == 1)
    {
        Serial.print(F("server is at "));
        Serial.println(Ethernet.localIP());
        return true;
    }
    else
    {
        Serial.println(F("[Eth] DHCP failed"));
        return false;
    }
}

void response200(EthernetClient &client)
{
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));
}
void response404(EthernetClient &client)
{
    client.println(F("HTTP/1.1 404 NotFound"));
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));
}
void response500(EthernetClient &client)
{
    client.println(F("HTTP/1.1 500 Error"));
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));
}
void sendData(Aconfig &config)
{
    EthernetClient client = server.available();
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
                StaticJsonDocument<AconfigDocSize> httpResponse;
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
                else if (reqType.equals("POST"))
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
