#include "Hardware.h"

void Hardware::servoOpen(int n){
  Servo s;
  s.attach(14);
  s.write(0);
  delay(50);
  for(int i = 0;i<n;i++){
    s.write(120);
    delay(1000);
    s.write(0);
    delay(500);
  }
  s.detach();
}

void Hardware::servoZero() {
  Servo s;
  s.attach(14);
  s.write(0);
  delay(100);
  s.detach();
}
