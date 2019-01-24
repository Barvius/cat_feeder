#include "Discovering.h"

Discovering::Discovering(Logger* logger){
  this->logger = logger;
}

void Discovering::init(){
  this->udp = new WiFiUDP();
  this->udp->begin(11111);
}

void Discovering::loop(){
  // char packetBuffer[255];

  if (this->udp->parsePacket()) {
    int packetLen = this->udp->available();
    this->logger->writeLn("recv");
    this->logger->writeLn(String(this->udp->remoteIP()));
    this->logger->writeLn(String(this->udp->remotePort()));
    char packetData[255];
    this->udp->read(packetData,packetLen);
    if (!strcmp(packetData, "M-SEARCH * HTTP/1.1\r\n")){
      this->logger->writeLn("nash packet");
      this->udp->beginPacket(this->udp->remoteIP(), this->udp->remotePort());
      this->udp->write("HTTP/1.1 200");
      this->udp->endPacket();
    }

    // Serial.print("Received packet of size ");
    // Serial.println(packetSize);
    // Serial.print("From ");
    // IPAddress remoteIp = this->udp->remoteIP();
    // Serial.print(remoteIp);
    // Serial.print(", port ");
    // Serial.println(this->udp->remotePort());
    // int len = this->udp->read(packetBuffer, 255);
    // if (len > 0) {
    //   packetBuffer[len] = 0;
    // }
    // Serial.println("Contents:");
    // Serial.println(packetBuffer);

  }
}
