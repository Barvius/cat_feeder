#include "RTC.h"

RTC::RTC(){}

boolean RTC::isRunning() {
  Wire.beginTransmission(0x68);
  Wire.write((byte)0);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  uint8_t ss = Wire.read();
  return !(ss>>7);
}

boolean RTC::init(){
  Wire.begin();
  return true;
}

std::tm RTC::getTime(){
std::tm time = {0};
  Wire.beginTransmission(0x68);
  Wire.write((byte)0);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 7);
  time.tm_sec = bcd2bin(Wire.read() & 0x7F);
  time.tm_min = bcd2bin(Wire.read());
  time.tm_hour = bcd2bin(Wire.read());
   // bcd2bin(Wire.read());
  time.tm_wday = bcd2bin(Wire.read());
  time.tm_mday = bcd2bin(Wire.read()) - 1;
  time.tm_mon = bcd2bin(Wire.read()) - 1;
  time.tm_year = bcd2bin(Wire.read()) + 100;
  return time;
}

void RTC::setTime(std::tm time){
  // Logger::getInstance()->writeLn("Writing time to RTC");
  Wire.beginTransmission(0x68);
  Wire.write((byte)0);
  Wire.write(bin2bcd(time.tm_sec));
  Wire.write(bin2bcd(time.tm_min));
  Wire.write(bin2bcd(time.tm_hour));
  Wire.write(bin2bcd(time.tm_wday));
  Wire.write(bin2bcd(time.tm_mday) + 1);
  Wire.write(bin2bcd(time.tm_mon) + 1);
  Wire.write(bin2bcd(time.tm_year - 100));
  Wire.endTransmission();
}
