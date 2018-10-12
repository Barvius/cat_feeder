void HTTP_init(void) {
  HTTP.on("/restart", restart);
  HTTP.on("/feed", feed_handler);
  HTTP.begin();
}


void feed_handler(){

  myServo.attach(14);
  myServo.write(0);
  delay(50);
  HTTP.send(200, "text / plain", "OK");
  for(int i = 0;i<6;i++){ //~10g
    myServo.write(120);
    delay(1000);
    myServo.write(0);
    delay(500);
  }
  myServo.detach();
}


void restart() {
  HTTP.send(200, "text / plain", "Reset OK");
  ESP.restart();
}

