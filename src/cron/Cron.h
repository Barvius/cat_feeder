#ifndef cron_h
#define cron_h

#include <vector>
#include <ctime>
#include <ESP8266HTTPClient.h>
#include "Arduino.h"
#include "../task/Task.h"
#include "../servo/ServoController.h"

class Cron {
private:
  std::vector<Task> task;
  unsigned long lastTickTime;
  boolean ntpTimeActive;
  unsigned long ntpTimeCheck;

  boolean checkTaskTime(Task,time_t);
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
