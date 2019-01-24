#ifndef wireless_h
#define wireless_h

#include <ESP8266WiFi.h>
#include "../logger/Logger.h"

class Wireless {
private:
  Logger* logger;
  enum WirelessMode {AP, STA};
  WirelessMode wirelessMode;

  unsigned long lastTickTime;

  void connectToAP();
  void startAP();



public:
  Wireless(Logger*);
  void init();
  void loop();
};

#endif
