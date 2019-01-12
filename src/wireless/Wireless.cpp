#include "Wireless.h"

void Wireless::connectToAP(){
  Logger::getInstance()->writeLn("Connecting to "+WiFi.SSID());
  this->wirelessMode = STA;
  WiFi.mode(WIFI_STA);
  WiFi.begin();
}

void Wireless::startAP(){
  Logger::getInstance()->writeLn("Start AP");
  this->wirelessMode = AP;
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(String("ESP_" + String(ESP.getChipId())).c_str(), String("ESP_" + String(ESP.getChipId())).c_str());
}

void Wireless::init(){
  if(WiFi.SSID().equals("") && WiFi.psk().equals("")){
    this->startAP();
  } else {
    this->connectToAP();
  }
}

void Wireless::loop(){
  if(millis() - this->lastTickTime > 30000){
    switch (this->wirelessMode) {
      case AP:
        if(WiFi.status() == WL_CONNECTED && wifi_softap_get_station_num() == NULL){
          Logger::getInstance()->writeLn("Resume connect to router");
          this->connectToAP();
        }
        break;
      case STA:
        if(WiFi.status() != WL_CONNECTED){
          Logger::getInstance()->writeLn("Failed connect to router");
          this->startAP();
        }
        break;
    }
    this->lastTickTime = millis();
  }

}

Wireless::Wireless(){
  this->lastTickTime = 0;
}

Wireless* Wireless::instance = nullptr;

Wireless* Wireless::getInstance(){
  if (instance == nullptr)
    instance = new Wireless;
   return instance;
}
