#ifndef http_h
#define http_h

#include "Arduino.h"
#include <time.h>
#include <ESP8266WebServer.h>
#include "../servo/ServoController.h"
#include "../cron/Cron.h"

class HTTP {
private:
  static HTTP *instance;
  ESP8266WebServer *http;
  HTTP ();

  void task_list_handler();
  void task_add_handler();
  void task_edit_handler();
  void task_del_handler();

  void available_networks_handler();
  void restart_handler();
  void connect_handler();
  // void time_handler();
  void feed_handler();

public:
  static HTTP *getInstance();
  void init();
  void handleClient();

};
#endif
