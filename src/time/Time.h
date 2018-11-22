#ifndef time_h
#define time_h

#include <ctime>
#include <string>
#include "Arduino.h"

class Time {
public:

  static String getCurrentTime(){
    time_t tm = time(nullptr);
    return strtok(ctime(&tm), "\n");
  }

  static void configTZ(int tz){
    configTime(tz * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");
  }

};

#endif
