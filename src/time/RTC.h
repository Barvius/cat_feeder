#ifndef rtc_h
#define rtc_h

#include <Arduino.h>
#include <Wire.h>
#include <ctime>

class RTC {
private:
  RTC();
public:

static std::tm getTime();
static void setTime(std::tm);
static boolean init();
static boolean isRunning();
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
};

#endif
