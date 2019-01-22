#include "Time.h"

Time::Time(){
  if (! this->beginRTC()) {
    Logger::getInstance()->writeLn("Couldn't find RTC");
  }
  if (!this->isRunningRTC()) {
    Logger::getInstance()->writeLn("RTC is NOT running!");
  }
}

Time* Time::instance = nullptr;

Time* Time::getInstance(){
  if (instance == nullptr)
    instance = new Time;
   return instance;
}
