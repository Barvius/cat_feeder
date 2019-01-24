#include "Cron.h"

Cron::Cron(){
  this->lastTickTime = (60 - RTC::getTime().tm_sec) * 1000;
}

void Cron::init(){
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

boolean Cron::checkTaskTime(Task task, std::tm now){
  if(task.getHours() == now.tm_hour && task.getMinutes() == now.tm_min){
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
  if (RTC::isRunning()) {
    if(millis() - this->lastTickTime > 1000*60){
      for (Task i : this->task) {
        if (this->checkTaskTime(i,RTC::getTime())) {
          Logger::getInstance()->writeLn("Start feed " + String(i.getWeight()) +"gr");
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
