#ifndef servo_h
#define servo_h

#include <Servo.h>

class ServoController {
private:
  Servo servo;

  boolean feeding;
  boolean open;
  unsigned long lastTime;
  unsigned int iteration;

  void startFeed(unsigned int);
  void stopFeed();

  static ServoController *instance;
  ServoController();

public:
  static ServoController *getInstance();
  void loop();
  void init();
  boolean feed(unsigned int);
};
#endif
