#ifndef logger_h
#define logger_h

#include "Arduino.h"
#include <ctime>
#include "../time/RTC.h"

class Logger {
private:
  String log;
  static Logger *instance;
  Logger();

public:
  static Logger *getInstance();
  void write(String);
  void writeLn(String);
  String getLog();
  String getCurrentTime(){
    // time_t tm = time(nullptr);
    std::tm time;
    time = RTC::getTime();
    return strtok(asctime(&time), "\n");
  }
};
#endif
