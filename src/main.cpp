#include <Arduino.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "secrets.h"

const char *ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4="
    "-----END CERTIFICATE-----\n";

WiFiMulti wifiMulti;
WiFiClientSecure espClient;
PubSubClient client(espClient);

const char *topic = MQTT_TOPIC;

void setupWifi()
{
    delay(100);
    Serial.print("\nConneting to ");
    Serial.println(WIFI_SSID);

    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
    while (wifiMulti.run() != WL_CONNECTED)
    {
        delay(100);
        Serial.print("-");
    }

    Serial.print("\nConnected to");
    Serial.println(WIFI_SSID);
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("\nConnecting to ");
        Serial.println(MQTT_SERVER);

        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());

        if (client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
        {
            Serial.print("\nConnected to ");
            Serial.println(MQTT_SERVER);
            Serial.printf("ClientId %s", client_id.c_str());
            client.publish(topic, "Hi, I'm ESP32 ^^");
        }
        else
        {
            Serial.println("\nTrying to connect....");
            Serial.print(client.state());
            delay(5000);
        }
    }
}

void setup() {
  Serial.begin(921600);
  Serial.print("\nStarting ESP board.....");

  pinMode(LED_BUILTIN, OUTPUT);
  setupWifi();
  espClient.setCACert(ca_cert);
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop(){
  digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED);
  if (!client.connected())
  {
      reconnect();
  }
  client.loop();
}