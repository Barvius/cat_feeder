#ifndef http_h
#define http_h

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include "../servo/ServoController.h"
#include "../cron/Cron.h"
#include "../logger/Logger.h"
#include "../time/Time.h"
#include <FS.h>

class HTTP {
private:
  RTC* rtc = nullptr;
  Logger* logger = nullptr;
  Cron* cron = nullptr;
  ServoController* servoController = nullptr;

  ESP8266WebServer *http;


  void task_list_handler();
  void task_add_handler();
  void task_edit_handler();
  void task_del_handler();

  void log_handler();

  void available_networks_handler();
  void restart_handler();
  void connect_wifi_handler();
  void info_wifi_handler();
  void feed_handler();
  void time_handler();
  void ntpTimeHandler();

  String FSContentType(String);
  boolean FSFileRead(String);
  void FSFileList();
  void FSFileUpload();
  void FSFileDelete();
  void FSFileCreate();
public:
  HTTP (Logger*,RTC*,Cron*,ServoController*);
  void init();
  void handleClient();

};
#endif
