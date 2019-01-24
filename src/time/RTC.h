#ifndef RTC_h
#define RTC_h

#include <ctime>
#include <Wire.h>
#include <Arduino.h>
#include "RTCStruct.h"

class RTC {
private:

public:
  RTC();
  std::tm getTime();
  void setTime(std::tm);
  boolean init();
  boolean isRunning();
  uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
  uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
};

#endif
