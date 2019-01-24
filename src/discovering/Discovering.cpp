#include "Discovering.h"

Discovering::Discovering(){}

void Discovering::init(){
  this->udp = new WiFiUDP();
  this->udp->begin(11111);
}

void Discovering::loop(){
  if (this->udp->parsePacket()) {
    int packetLen = this->udp->available();
    char packetData[255];
    this->udp->read(packetData,packetLen);
    if (!strcmp(packetData, "M-SEARCH * HTTP/1.1")){
      // Logger::getInstance()->writeLn("nash packet");
      this->udp->beginPacket(this->udp->remoteIP(), this->udp->remotePort());
      this->udp->write("HTTP/1.1 200");
      this->udp->endPacket();
    }
  }
}

Discovering* Discovering::instance = nullptr;

Discovering* Discovering::getInstance(){
  if (instance == nullptr){
    instance = new Discovering;
  }
  return instance;
}
