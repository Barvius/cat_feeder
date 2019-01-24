#ifndef logger_h
#define logger_h

// #include "Arduino.h"
#include <ctime>
#include "../time/RTC.h"
#include "../time/RTCStruct.h"

class Logger {
private:
  RTC *rtc = nullptr;
  String log;


public:
  Logger(RTC*);
  void write(String);
  void writeLn(String);
  String getLog();
  String getCurrentTime(){
    // std::tm time;
    // time = this->rtc->getTime();
    return "time here";
  }


};
#endif
