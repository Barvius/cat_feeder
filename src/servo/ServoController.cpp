#include "ServoController.h"

void ServoController::loop(){
  if(this->feeding){
    if(this->iteration > 0){
      if(millis() - this->lastTime > 1000){
        if(this->open){
          this->servo.write(0);
          this->open = false;
        }else{
          this->servo.write(120);
          this->open = true;
        }
        this->lastTime = millis();
        this->iteration--;
      }
    }else{
      this->stopFeed();
    }
  }
}

void ServoController::init(){
  this->servo.attach(14);
  this->servo.write(0);
  delay(50);
  this->servo.detach();
}

boolean ServoController::feed(unsigned int weight){
  switch (weight) {
    // case 5:
    // this->startFeed(5);
    //   return true;
    case 10:
    this->startFeed(7);
      return true;
    case 20:
    this->startFeed(13);
      return true;
    default:
      return false;
  }
}

void ServoController::startFeed(unsigned int iteration){
  this->servo.attach(14);
  this->servo.write(0);
  this->feeding = true;
  this->open = false;
  this->iteration = iteration;
  this->lastTime = 0;
}

void ServoController::stopFeed(){
  this->servo.detach();
  this->feeding = false;
}

ServoController::ServoController(){
  this->feeding = false;
  this->init();
}
