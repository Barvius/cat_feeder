#include "App.h"

App::App(){

  this->rtc = new RTC();
  this->logger = new Logger(rtc);
  this->discovering = new Discovering(logger);
  this->wireless = new Wireless(logger);
  this->servoController = new ServoController();
  this->cron = new Cron(rtc,logger,servoController);
  this->http = new HTTP(logger,rtc,cron,servoController);
}

void App::loop(){
  wireless->loop();
  cron->loop();
  servoController->loop();
  http->handleClient();
  discovering->loop();
  delay(1);
}
