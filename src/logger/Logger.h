#ifndef logger_h
#define logger_h

#include "Arduino.h"

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
};
#endif
