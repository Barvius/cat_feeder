#ifndef time_h
#define time_h

#include "../logger/Logger.h"
#include "RTC.h"
#include <ctime>
#include <string>
#include "Arduino.h"

#include <WiFiUdp.h>

class Time {
private:
  static Time *instance;
  Time();
  void writeNTPPacket(WiFiUDP& , String);
  time_t readNTPPacket(WiFiUDP& , int);

public:

  static Time *getInstance();

  String getTime(){
    std::tm t;
    // RTC::getTime(&t);
    // Logger::getInstance()->writeLn(String(t.tm_mday));
    // return String(String(t.tm_hour)+":"+String(t.tm_min)+":"+String(t.tm_sec));
    return String(std::asctime(&t));
  }


  boolean getTimeNTP(String, int);
  // std::tm getCurrentTime();


  // static String getCurrentTime(){
  //   time_t tm = time(nullptr);
  //   return strtok(ctime(&tm), "\n");
  // }

  // static void configTZ(int tz){
  //   configTime(tz * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");
  // }

};

#endif
