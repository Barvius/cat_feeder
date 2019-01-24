#ifndef app_h
#define app_h

#include "../cron/Cron.h"
#include "../discovering/Discovering.h"
#include "../http/Http.h"
#include "../logger/Logger.h"
#include "../servo/ServoController.h"
#include "../time/RTC.h"
#include "../wireless/Wireless.h"

class App {
private:
  RTC* rtc;
  Logger* logger;
  Discovering* discovering;
  Wireless* wireless;
  ServoController* servoController;
  Cron* cron;
  HTTP* http;




public:
  App();
  void loop();
};






#endif
