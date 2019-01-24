#ifndef ntp_h
#define ntp_h

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ctime>
#include "../logger/Logger.h"

class NTP {
private:
  NTP();
  static void writeNTPPacket(WiFiUDP& , String);
  static time_t readNTPPacket(WiFiUDP& , int);
public:
  static boolean synchronize(String,int);
};

#endif
