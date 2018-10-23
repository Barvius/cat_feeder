#ifndef cron_h
#define cron_h

#include <vector>
#include "Arduino.h"

#include "Task.h"

class Cron {
private:
  std::vector<Task> task;
  unsigned long lastTickTime;
  boolean ntpTimeActive;

  void readConfig();
  void writeConfig();

  static Cron *instance;
  Cron();

public:
  static Cron *getInstance();
  void loop();
  void addTask(Task);
  void delTask(unsigned int);
};

#endif
