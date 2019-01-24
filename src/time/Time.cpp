#include "Time.h"

Time::Time(){

}

// std::tm Time::getCurrentTime(){
//     return std::tm;
// }

void Time::writeNTPPacket(WiFiUDP &udp, String server){
  uint8_t ntpPacket[48];
  memset (ntpPacket, 0, 48);
  ntpPacket[0] = 0b11100011;   // LI, Version, Mode
  ntpPacket[1] = 0;     // Stratum, or type of clock
  ntpPacket[2] = 6;     // Polling Interval
  ntpPacket[3] = 0xEC;  // Peer Clock Precision
                              // 8 bytes of zero for Root Delay & Root Dispersion
  ntpPacket[12] = 49;
  ntpPacket[13] = 0x4E;
  ntpPacket[14] = 49;
  ntpPacket[15] = 52;

  udp.beginPacket (server.c_str(), 123);
  udp.write (ntpPacket, 48);
  udp.endPacket ();
}

time_t Time::readNTPPacket(WiFiUDP &udp, int offset){
  byte ntpPacketBuffer[48];
  //->writeLn("Receive NTP Response");
  udp.read(ntpPacketBuffer, 48);
  unsigned long secsSince1900;
  // convert four bytes starting at location 40 to a long integer
  secsSince1900 = (unsigned long)ntpPacketBuffer[40] << 24;
  secsSince1900 |= (unsigned long)ntpPacketBuffer[41] << 16;
  secsSince1900 |= (unsigned long)ntpPacketBuffer[42] << 8;
  secsSince1900 |= (unsigned long)ntpPacketBuffer[43];

  time_t timeTemp = (secsSince1900 - 2208988800UL) +(3600*offset);
  return timeTemp;
}

boolean Time::getTimeNTP(String server, int offset) {
  WiFiUDP udp;
  udp.begin(123);
  writeNTPPacket(udp,server);
  //->writeLn("Synchronization with the NTP server started");
  uint32_t beginWait = millis ();
  while (millis () - beginWait < 1500) {
      if (udp.available() >= 48) {
        time_t t = readNTPPacket(udp,offset);
        if(std::localtime(&t)){
            std::tm time;
            time = *std::localtime(&t);
            //->write("Current time set to "+String(std::asctime (&time)));
            // RTC::setTime(time);
            udp.stop ();
            return true;
        }
      }
      ESP.wdtFeed();
  }
  //->writeLn("Failed to find NTP time");
  udp.stop ();
  return false;
}

Time* Time::instance = nullptr;

Time* Time::getInstance(){
  if (instance == nullptr)
    instance = new Time;
   return instance;
}
