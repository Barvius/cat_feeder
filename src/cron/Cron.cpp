#include "Cron.h"

void Cron::loop(){
  if (this->ntpTimeActive) {
    if(millis() - this->lastTickTime > 1000){
      for (Task i : this->task) {
        
      }
      this->lastTickTime = millis();
    }
  }
}

void Cron::addTask(Task task){
  this->task.push_back(task);
}

void Cron::delTask(unsigned int index){
  this->task.erase(this->task.begin() + index);
}

Cron* Cron::instance = nullptr;

Cron* Cron::getInstance(){
  if (instance == nullptr)
    instance = new Cron;
   return instance;
}
