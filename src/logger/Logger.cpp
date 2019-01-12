#include "Logger.h"

Logger::Logger(){

}

void Logger::write(String str){
  this->log += getCurrentTime();
  this->log += " - ";
  this->log += str;
}

void Logger::writeLn(String str){
  this->log += getCurrentTime();
  this->log += " - ";
  this->log += str;
  this->log += "\n";
}

String Logger::getLog(){
  return this->log;
}

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance(){
  if (instance == nullptr)
    instance = new Logger;
   return instance;
}
