#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>

#include "http/Http.h"
#include "servo/ServoController.h"
#include "wireless/Wireless.h"

// void ConnectWiFi() {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin();
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

void UpdateFirmware() {
  t_httpUpdate_return ret = ESPhttpUpdate.update("192.168.1.141", 80, "/esp8266/", String(ESP.getSketchSize()));
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}

void setup() {
  Serial.begin(115200);

  Wireless::getInstance()->init();
  int res = WiFi.waitForConnectResult();
  if(res == WL_CONNECTED){
    UpdateFirmware();
  }

  Cron::getInstance()->init();
  HTTP::getInstance()->init();
}

void loop() {
  Wireless::getInstance()->loop();
  Cron::getInstance()->loop();
  ServoController::getInstance()->loop();
  HTTP::getInstance()->handleClient();
  delay(1);
}
