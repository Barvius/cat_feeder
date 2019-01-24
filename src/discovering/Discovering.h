#ifndef discovering_h
#define discovering_h

#include <WiFiUdp.h>
#include "../logger/Logger.h"
class Discovering {
private:
  WiFiUDP *udp;
  static Discovering *instance;
  Discovering();
public:
  void init();
  void loop();
  static Discovering *getInstance();
};

#endif
