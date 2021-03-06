#ifndef cron_h
#define cron_h

#include <vector>
#include <ctime>
#include <ESP8266HTTPClient.h>
#include "Arduino.h"
#include "FS.h"
#include <stdio.h>
#include "../task/Task.h"
#include "../servo/ServoController.h"
#include "../logger/Logger.h"
#include "../time/RTC.h"

class Cron {
private:
  std::vector<Task> task;
  unsigned long lastTickTime;

  boolean checkTaskTime(Task,std::tm);
  void readConfig();
  void writeConfig();

  static Cron *instance;
  Cron();

public:
  void clear(){
    this->task.clear();
    writeConfig();
  }
  void init();
  String getStr();
  static Cron *getInstance();
  void loop();
  boolean addTask(Task);
  boolean delTask(unsigned int);
  boolean editTask(unsigned int, unsigned int);
};

#endif
