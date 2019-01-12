#include "Time.h"

Time::Time(){

}

Time* Time::instance = nullptr;

Time* Time::getInstance(){
  if (instance == nullptr)
    instance = new Time;
   return instance;
}
