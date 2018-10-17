#ifndef http_h
#define http_h

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include "../hardware/Hardware.h"

class HTTP {
private:
  static HTTP *instance;
  ESP8266WebServer *http;
  HTTP ();

  void available_networks_handler();
  void restart_handler();
  // void connect_handler();
  void feed_handler();


public:
  static HTTP *getInstance();
  void init();
  void handleClient();
};


#endif
