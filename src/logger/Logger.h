#ifndef logger_h
#define logger_h

#include "Arduino.h"
#include <ctime>
//#include "../Time/Time.h"

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
    time_t tm = time(nullptr);
    return strtok(ctime(&tm), "\n");
  }
};
#endif
