#include "Http.h"

HTTP::HTTP(){
  this->http = new ESP8266WebServer(80);
}

void HTTP::init(){
  // this->http->on("/*", [this]() {
  //   if (!this->http->authenticate("cat", "feeder")) {
  //     return this->http->requestAuthentication();
  //   }
  //   this->http->send(200, "text/plain", "Login OK");
  // });
  this->http->on("/feed", std::bind(&HTTP::feed_handler, this));
  this->http->on("/task/list", std::bind(&HTTP::task_list_handler, this));
  this->http->on("/task/add", std::bind(&HTTP::task_add_handler, this));
  this->http->on("/task/edit", std::bind(&HTTP::task_edit_handler, this));
  this->http->on("/task/del", std::bind(&HTTP::task_del_handler, this));

  this->http->on("/log", std::bind(&HTTP::log_handler, this));

  this->http->on("/available_networks", std::bind(&HTTP::available_networks_handler, this));
  // this->http->on("/wifi", std::bind(&HTTP::connect_handler, this));
  // this->http->on("/time", std::bind(&HTTP::time_handler, this));
  this->http->on("/restart", std::bind(&HTTP::restart_handler, this));
  this->http->begin();
}

void HTTP::task_list_handler(){
  this->http->send(200, "application/json", Cron::getInstance()->getStr());
}

void HTTP::task_add_handler(){
  if (this->http->argName(0) == "h" && this->http->argName(1) == "m" && this->http->argName(2) == "w") {
    if(!Cron::getInstance()->addTask(Task(this->http->arg("h").toInt(),this->http->arg("m").toInt(),this->http->arg("w").toInt()))){
      this->http->send(400, "text / plain","Add task failed");
    }
    this->http->send(200, "text / plain","Task successfully added");
  }
  this->http->send(400, "text / plain", "Bad Request");
}

void HTTP::task_edit_handler(){
  if (this->http->argName(0) == "id" && this->http->argName(1) == "w") {
    if(!Cron::getInstance()->editTask(this->http->arg("id").toInt(),this->http->arg("w").toInt())){
      this->http->send(400, "text / plain", "Task not found");
    }
    this->http->send(200, "text / plain","Task successfully changed");
  }
  this->http->send(400, "text / plain", "Bad Request");
}

void HTTP::task_del_handler(){
  if (this->http->argName(0) == "id"){
    if(!Cron::getInstance()->delTask(this->http->arg("id").toInt())){
      this->http->send(400, "text / plain", "Task not found");
    }
    this->http->send(200, "text / plain", "Task successfully deleted");
  }
  this->http->send(400, "text / plain", "Bad Request");
}

void HTTP::log_handler(){
  this->http->send(200, "text / plain", Logger::getInstance()->getLog());
}

void HTTP::handleClient(){
  this->http->handleClient();
}

void HTTP::feed_handler(){
  if (this->http->argName(0) == "amount") {
    if(ServoController::getInstance()->feed(this->http->arg("amount").toInt())){
      this->http->send(200, "text / plain", "FEED OK");
    }
  }
  this->http->send(400, "text / plain", "Bad Request");
}

void HTTP::available_networks_handler(){
  String json = "[";
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    if (i) {
      json += ",";
    }
    json += "{";
    json += "\"ssid\":\"";
    json += WiFi.SSID(i);
    json += "\",\"rssi\":";
    json += WiFi.RSSI(i);
    json += ",\"access\":\"";
    json += WiFi.encryptionType(i) == 0 ? "open":"close";
    json += "\"}";
    delay(10);
  }
  json += "]";
  this->http->send(200, "application/json", json);
}

void HTTP::restart_handler() {
  this->http->send(200, "text / plain", "Reset OK");
  delay(500);
  ESP.restart();
}

// void HTTP::time_handler(){
  // String json = "{";
  // json += GetTime(GetUnixTime()).c_str();
  // json+="}";
  // this->http->send(200, "text / plain", GetTime(GetUnixTime()).c_str());
// }

void HTTP::connect_handler() {
 // if (this->http->argName(0) == "ssid" && this->http->argName(1) == "password") {
 //   // WiFi.mode(WIFI_STA);
 //   WiFi.disconnect();
 //   WiFi.begin(this->http->arg("ssid").c_str(), this->http->arg("password").c_str());
 //   unsigned long timer = millis();
 //   while (WiFi.status() != WL_CONNECTED && millis() - timer < 10000) {
 //
 //   }
 //   if (millis() - timer >= 10000) {
 //     this->http->send(403, "text/plain", String(WiFi.status()));
 //     // break;
 //   }
 //   this->http->send(200, "text/plain", String(WiFi.status()));
 //   delay(500);
 //   ESP.restart();
 // }
 // this->http->send(400, "text / plain", "Bad Request");
}

HTTP* HTTP::instance = nullptr;

HTTP* HTTP::getInstance(){
  if (instance == nullptr)
    instance = new HTTP;
   return instance;
}
