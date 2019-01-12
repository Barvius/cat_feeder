#ifndef wireless_h
#define wireless_h

#include <ESP8266WiFi.h>
#include "../logger/Logger.h"

class Wireless {
private:
  enum WirelessMode {AP, STA};
  WirelessMode wirelessMode;

  unsigned long lastTickTime;

  void connectToAP();
  void startAP();

  static Wireless *instance;
  Wireless();

public:
  static Wireless *getInstance();
  void init();
  void loop();
};

#endif
