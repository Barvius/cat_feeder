#include "Logger.h"

Logger::Logger(RTC* rtc){
  this->rtc = rtc;
}

void Logger::write(String str){
  this->log += this->getCurrentTime();
  this->log += " - ";
  this->log += str;
}

void Logger::writeLn(String str){
  this->log += this->getCurrentTime();
  this->log += " - ";
  this->log += str;
  this->log += "\n";
}

String Logger::getLog(){
  return this->log;
}
