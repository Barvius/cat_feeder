#include "Cron.h"
#include "FS.h"
#include <stdio.h>

Cron::Cron(){
  this->lastTickTime = 0;
  this->ntpTimeCheck = 0;
  this->ntpTimeActive = false;
}

void Cron::init(){
  configTime(3 * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");
  if(!SPIFFS.begin()){
  }
  this->readConfig();
}

String Cron::getStr(){
  String s = "[";
  for (size_t i = 0; i < this->task.size(); i++) {
    if(i > 0){
      s += ",[";
    } else {
      s += "[";
    }
    s += this->task.at(i).getHours();
    s += ",";
    s += this->task.at(i).getMinutes();
    s += ",";
    s += this->task.at(i).getWeight();
    s += "]";
  }
  s+="]";
  return s;
}

boolean Cron::checkTaskTime(Task task, time_t now){
  String Time = ctime(&now);
  int i = Time.indexOf(":");
  String h = Time.substring(i - 2, i);
  String m = Time.substring(i+1, i + 3);
  Serial.println(h.toInt());
  Serial.println(m.toInt());
  if(task.getHours() == h.toInt() && task.getMinutes() == m.toInt()){
    return true;
  }
  return false;
}

void Cron::readConfig(){
  File f = SPIFFS.open("/cron.json", "r");
  String s;
  while(f.available()){

    s = f.readStringUntil('\n');
    int ind1 = s.indexOf(':');
    String h = s.substring(0, ind1);
    int ind2 = s.indexOf(':', ind1+1 );
    String m = s.substring(ind1+1, ind2+1);
    String weight = s.substring(ind2+1, s.length());

    this->task.push_back(Task(h.toInt(),m.toInt(),weight.toInt()));

    }
  f.close();
}

void Cron::writeConfig(){
  File f = SPIFFS.open("/cron.json", "w");
  for (Task i : this->task) {
    f.print(i.getHours());
    f.print(":");
    f.print(i.getMinutes());
    f.print(":");
    f.println(i.getWeight());
  }
  f.close();
}

void Cron::loop(){
  if (this->ntpTimeActive) {
    if(millis() - this->lastTickTime > 1000*60){
      for (Task i : this->task) {
        if (this->checkTaskTime(i,time(nullptr))) {
          Logger::getInstance()->writeLn("Start feed");
          ServoController::getInstance()->feed(i.getWeight());
          HTTPClient httpClient;
          httpClient.setUserAgent("FEEDER");
          httpClient.begin("http://192.168.1.141/"+String(i.getWeight()));
          httpClient.GET();
          httpClient.end();
        }
      }
      this->lastTickTime = millis();
    }
  }
  if(millis() - this->ntpTimeCheck > 10000){
    if (!time(nullptr)) {
      Logger::getInstance()->writeLn("Ntp Server is down");
      this->ntpTimeActive = false;
    } else {
      this->ntpTimeActive = true;
    }
    this->ntpTimeCheck = millis();
  }
}

boolean Cron::addTask(Task task){
  if(!task.isValid()){
    return false;
  }
  this->task.push_back(task);
  this->writeConfig();
  return true;
}

boolean Cron::delTask(unsigned int index){
  if(index > this->task.size() - 1){
    return false;
  }
  this->task.erase(this->task.begin() + index, this->task.begin() + index + 1);
  this->writeConfig();
  return true;
}

boolean Cron::editTask(unsigned int index, unsigned int weight){
  if(index > this->task.size() - 1){
    return false;
  }
  this->task.at(index).setWeight(weight);
  this->writeConfig();
  return true;
}

Cron* Cron::instance = nullptr;

Cron* Cron::getInstance(){
  if (instance == nullptr)
    instance = new Cron;
   return instance;
}
