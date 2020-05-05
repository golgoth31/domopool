#include "Aethernet.h"

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

bool startEthernetServer(Network conf)
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
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        return false;
    }
    if (Ethernet.linkStatus() == LinkOFF) // not working on W5100 shield
    {
        Serial.println("Ethernet cable is not connected.");
        return false;
    }

    // start the server
    if (ethOK == 1)
    {
        Serial.print("server is at ");
        Serial.println(Ethernet.localIP());
        return true;
    }
    else
    {
        Serial.println("[Eth] DHCP failed");
        return false;
    }
}

void sendData(EthernetServer server, Config &config)
{
    EthernetClient client = server.available();
    bool postRequest = false;
    if (client)
    {
        Serial.println("[Ethernet] New client request");
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

                if (reqType.equals("GET"))
                {
                    StaticJsonDocument<AconfigDocSize> httpResponse;
                    convert2doc(config, httpResponse);

                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: application/json");
                    client.println("Connection: close");
                    client.print(F("Content-Length: "));
                    client.println(measureJsonPretty(httpResponse));
                    client.println();

                    serializeJsonPretty(httpResponse, client);
                }
                else if (reqType.equals("POST"))
                {
                    bool saved = saveJson(reqBody, config, "config.jsn");
                    if (saved)
                    {
                        postRequest = true;
                    }
                }
            }
            // give the web browser time to receive the data
            delay(1);
            // close the connection:
            client.stop();
            Serial.println("[Ethernet] Response sent");
            if (postRequest)
            {
                Serial.println("[Ethernet] New configuration received, rebooting");
                software_Reboot();
            }
        }
    }
}
