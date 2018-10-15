void HTTP_init(void) {
  HTTP.on("/restart", restart);
  HTTP.on("/feed", feed_handler);
  HTTP.begin();
}

void feed_handler(){
  if (HTTP.argName(0) == "amount") {
    switch(HTTP.arg("amount").toInt()){
      case 5:
        HTTP.send(200, "text / plain", "FEED OK");
        feed(3);
      break;
      case 10:
        HTTP.send(200, "text / plain", "FEED OK");
        feed(6);
      break;
      case 15:
        HTTP.send(200, "text / plain", "FEED OK");
        feed(9);
      break;
      default:
        HTTP.send(400, "text / plain", "Bad Request");
    }
  } else {
    HTTP.send(400, "text / plain", "Bad Request");
  }
}

void feed(int n){
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  delay(50);
  for(int i = 0;i<n;i++){ //~10g
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

