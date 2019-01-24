#ifndef discovering_h
#define  discovering_h

#include <WiFiUdp.h>
#include "../logger/Logger.h"
class Discovering {
private:
  Logger* logger = nullptr;
  WiFiUDP *udp;
public:
  Discovering(Logger*);
  void init();
  void loop();
};

#endif
