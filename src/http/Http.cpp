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
  this->http->on("/time", std::bind(&HTTP::time_handler, this));
  this->http->on("/info", [this]() {
    String str="";
    str += "heap = ";
    str += ESP.getFreeHeap();
    this->http->send(200, "text/plain", str);
  });

  this->http->on("/available_networks", std::bind(&HTTP::available_networks_handler, this));
  this->http->on("/wifi", HTTP_POST, std::bind(&HTTP::connect_wifi_handler, this));
  this->http->on("/wifi", HTTP_GET, std::bind(&HTTP::info_wifi_handler, this));
  // this->http->on("/time", std::bind(&HTTP::time_handler, this));
  this->http->on("/restart", std::bind(&HTTP::restart_handler, this));

  this->http->on("/list", HTTP_GET, std::bind(&HTTP::FSFileList, this));
  this->http->on("/edit", HTTP_PUT, std::bind(&HTTP::FSFileCreate, this));
  //delete file
  this->http->on("/edit", HTTP_DELETE, std::bind(&HTTP::FSFileDelete, this));
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  this->http->on("/edit", HTTP_POST, [this]() {
    this->http->send(200, "text/plain", "");
  }, std::bind(&HTTP::FSFileUpload, this));

  this->http->onNotFound([this]() {
    if (!this->FSFileRead(this->http->uri())) {
      this->http->send(404, "text/plain", "FileNotFound");
    }
  });


  this->http->begin();
}

String HTTP::FSContentType(String filename) {
  if (this->http->hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

boolean HTTP::FSFileRead(String path) {
  // DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = this->FSContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz)) {
      path += ".gz";
    }
    File file = SPIFFS.open(path, "r");
    this->http->streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void HTTP::FSFileList() {

  this->http->sendHeader("Access-Control-Allow-Origin", "*", true);
  if (!this->http->hasArg("dir")) {
    this->http->send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = this->http->arg("dir");
  // DBG_OUTPUT_PORT.println("handleFileList: " + path);
  Dir dir = SPIFFS.openDir(path);
  path = String();

  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") {
      output += ',';
    }
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }

  output += "]";
  this->http->send(200, "text/json", output);
}
File fsUploadFile;
void HTTP::FSFileUpload() {
  //this->http->sendHeader("Access-Control-Allow-Origin", "*", true);
Logger::getInstance()->writeLn("upload");
  HTTPUpload& upload = this->http->upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Logger::getInstance()->writeLn("handleFileUpload Name: "); Logger::getInstance()->writeLn(filename);
    fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    Logger::getInstance()->writeLn("end");
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Logger::getInstance()->writeLn("handleFileUpload Size: "); Logger::getInstance()->writeLn(String(upload.totalSize));
      //this->http->sendHeader("Location","/success.html");      // Redirect the client to the success page
      this->http->send(200);
    } else {
      this->http->send(500, "text/plain", "500: couldn't create file");
    }
  }
}

void HTTP::FSFileDelete() {
  this->http->sendHeader("Access-Control-Allow-Origin", "*", true);
  if (this->http->args() == 0) {
    return this->http->send(500, "text/plain", "BAD ARGS");
  }
  String path = this->http->arg(0);
  // DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
  if (path == "/") {
    return this->http->send(500, "text/plain", "BAD PATH");
  }
  if (!SPIFFS.exists(path)) {
    return this->http->send(404, "text/plain", "FileNotFound");
  }
  SPIFFS.remove(path);
  this->http->send(200, "text/plain", "");
  path = String();
}

void HTTP::FSFileCreate() {
  if (this->http->args() == 0) {
    return this->http->send(500, "text/plain", "BAD ARGS");
  }
  String path = this->http->arg(0);
  // DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
  if (path == "/") {
    return this->http->send(500, "text/plain", "BAD PATH");
  }
  if (SPIFFS.exists(path)) {
    return this->http->send(500, "text/plain", "FILE EXISTS");
  }
  File file = SPIFFS.open(path, "w");
  if (file) {
    file.close();
  } else {
    return this->http->send(500, "text/plain", "CREATE FAILED");
  }
  this->http->send(200, "text/plain", "");
  path = String();
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
    json += "\",\"connect\":\"";
    json += WiFi.SSID() == WiFi.SSID(i) ? "true":"false";
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

void HTTP::connect_wifi_handler() {
 if (this->http->hasArg("ssid") && this->http->hasArg("password")) {
   // WiFi.mode(WIFI_STA);
   this->http->send(200, "application/json", "{'status':'reset'}");
   delay(500);
   WiFi.disconnect();
   WiFi.begin(this->http->arg("ssid").c_str(), this->http->arg("password").c_str());
   delay(500);
   // unsigned long timer = millis();
   // while (WiFi.status() != WL_CONNECTED && millis() - timer < 10000) {
   //
   // }
   // if (millis() - timer >= 10000) {
   //   this->http->send(403, "text/plain", String(WiFi.status()));
   //   // break;
   // }

   ESP.restart();
 }
 this->http->send(400, "text / plain", "Bad Request");
}

void HTTP::info_wifi_handler(){
  this->http->send(400, "text / plain", WiFi.localIP().toString());
}

void HTTP::time_handler(){
  if (this->http->argName(0) == "time") {
    this->http->send(200, "application/json", Time::getInstance()->getT(this->http->arg("time").toInt()));
  }
  this->http->send(200, "application/json", Time::getInstance()->getTime());
}
HTTP* HTTP::instance = nullptr;

HTTP* HTTP::getInstance(){
  if (instance == nullptr)
    instance = new HTTP;
   return instance;
}
