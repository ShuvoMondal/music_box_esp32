#include <Arduino.h>
#include <WiFiMulti.h>
#include "secrets.h"

WiFiMulti wifiMulti;

void setup() {
  Serial.begin(921600);
  Serial.println("Starting.....");

  pinMode(LED_BUILTIN, OUTPUT);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (wifiMulti.run() != WL_CONNECTED) {
    delay (100);
  }
  
  Serial.println("Connected");
}

void loop(){
  digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED);
}
