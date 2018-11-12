#include "Wireless.h"

void Wireless::connectToAP(){
  this->wirelessMode = STA;
  WiFi.mode(WIFI_STA);
  WiFi.begin();
}

void Wireless::startAP(){
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
        
        break;
      case STA:
        if(WiFi.status() != WL_CONNECTED){
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
