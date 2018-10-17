#ifndef hardware_h
#define hardware_h

#include "Arduino.h"
#include <Servo.h>

class Hardware {
public:
  static void servoOpen(int);
  static void servoZero();
};

#endif
