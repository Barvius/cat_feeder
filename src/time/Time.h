#ifndef time_h
#define time_h

#include "../logger/Logger.h"
#include <ctime>
#include <string>
#include "Arduino.h"
#include <Wire.h>

class Time {
private:
  static Time *instance;
  Time();
  uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
  uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

public:
  static Time *getInstance();

  String getT(unsigned int time){
    time_t t = time;
    setTimeRTC(std::gmtime(&t));
    return String(std::ctime(&t));
  }

  String getTime(){
    // GetNTP();
    if (! this->beginRTC()) {
      Serial.println("Couldn't find RTC");
      Logger::getInstance()->writeLn("Couldn't find RTC");
    }
    if (!this->isRunningRTC()) {
      //this->rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      Serial.println("RTC is NOT running!");
      Logger::getInstance()->writeLn("RTC is NOT running!");
    }
    //Logger::getInstance()->writeLn(String(this->rtc.now().minute()));
    std::tm t = getTimeRTC();
    //return String(String(t.tm_hour)+":"+String(t.tm_min)+":"+String(t.tm_sec));
    return String(std::asctime(&t));
  }

  std::tm getTimeRTC(){
    std::tm time = {0};
    Wire.beginTransmission(0x68);
    Wire.write((byte)0);
    Wire.endTransmission();

    Wire.requestFrom(0x68, 7);
    time.tm_sec = bcd2bin(Wire.read() & 0x7F);
    time.tm_min = bcd2bin(Wire.read());
    time.tm_hour = bcd2bin(Wire.read());
    time.tm_wday = bcd2bin(Wire.read());
    time.tm_mday = bcd2bin(Wire.read());
    time.tm_mon = bcd2bin(Wire.read());
    time.tm_year = bcd2bin(Wire.read()) + 100;

    return time;
  }

  void setTimeRTC(std::tm *time){
    Logger::getInstance()->writeLn("Setting rtc");
    Logger::getInstance()->writeLn(String((time->tm_hour+3)%24));
    Wire.beginTransmission(0x68);
    Wire.write((byte)0); // start at location 0
    Wire.write(bin2bcd(time->tm_sec));
    Wire.write(bin2bcd(time->tm_min));
    Wire.write(bin2bcd(time->tm_hour));
    Wire.write(bin2bcd(time->tm_wday));
    Wire.write(bin2bcd(time->tm_mday));
    Wire.write(bin2bcd(time->tm_mon));
    Wire.write(bin2bcd(time->tm_year - 100));

    Wire.endTransmission();
  }

  boolean beginRTC(){
    Wire.begin();
    return true;
  }

  uint8_t isRunningRTC() {
    Wire.beginTransmission(0x68);
    Wire.write((byte)0);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 1);
    uint8_t ss = Wire.read();
    return !(ss>>7);
  }

//   String decodeNtpMessage(byte *messageBuffer) {
//     std::tm time = {0};
//       unsigned long secsSince1900;
//       // convert four bytes starting at location 40 to a long integer
//       secsSince1900 = (unsigned long)messageBuffer[40] << 24;
//       secsSince1900 |= (unsigned long)messageBuffer[41] << 16;
//       secsSince1900 |= (unsigned long)messageBuffer[42] << 8;
//       secsSince1900 |= (unsigned long)messageBuffer[43];
//
//   #define SEVENTY_YEARS 2208988800UL
//       time_t timeTemp = secsSince1900 - SEVENTY_YEARS + 3 * 3600 + 0 * 60;
//
//       //time = std::localtime(&timeTemp);
//     // time.tm_sec = sec (timeTemp);;
//     // time.tm_min = min (timeTemp);
//     // time.tm_hour = hour (timeTemp);
//     // time.tm_wday = wday (timeTemp)+1;
//     // time.tm_mday = day (timeTemp);
//     // time.tm_mon = month (timeTemp) + 1;
//     // time.tm_year = year (timeTemp) - 1900;
//
//       return std::asctime(localtime(&timeTemp));
//   }
//
//   boolean sendNTPpacket (const char* address, UDP *udp) {
//     uint8_t ntpPacketBuffer[NTP_PACKET_SIZE]; //Buffer to store request message
//
//                                            // set all bytes in the buffer to 0
//     memset (ntpPacketBuffer, 0, NTP_PACKET_SIZE);
//     // Initialize values needed to form NTP request
//     // (see URL above for details on the packets)
//     ntpPacketBuffer[0] = 0b11100011;   // LI, Version, Mode
//     ntpPacketBuffer[1] = 0;     // Stratum, or type of clock
//     ntpPacketBuffer[2] = 6;     // Polling Interval
//     ntpPacketBuffer[3] = 0xEC;  // Peer Clock Precision
//                                 // 8 bytes of zero for Root Delay & Root Dispersion
//     ntpPacketBuffer[12] = 49;
//     ntpPacketBuffer[13] = 0x4E;
//     ntpPacketBuffer[14] = 49;
//     ntpPacketBuffer[15] = 52;
//     // all NTP fields have been given values, now
//     // you can send a packet requesting a timestamp:
//     udp->beginPacket (address, DEFAULT_NTP_PORT); //NTP requests are to port 123
//     udp->write (ntpPacketBuffer, NTP_PACKET_SIZE);
//     udp->endPacket ();
//     return true;
// }
//
//   boolean GetNTP(void) {
//     Logger::getInstance()->writeLn("Get NTP");
//     WiFiUDP udp;
//     byte ntpPacketBuffer[ NTP_PACKET_SIZE];
//     udp.begin(DEFAULT_NTP_PORT);
//     //iFi.hostByName(ntpServerName, timeServerIP);
//     sendNTPpacket(DEFAULT_NTP_SERVER,&udp);
//     uint32_t beginWait = millis ();
//     while (millis () - beginWait < NTP_TIMEOUT) {
//         int size = udp.parsePacket ();
//         Logger::getInstance()->writeLn("len = "+String(size));
//         if (size >= NTP_PACKET_SIZE) {
//             Logger::getInstance()->writeLn("-- Receive NTP Response");
//             //udp.read(ntpPacketBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
//             //Logger::getInstance()->writeLn(ntpPacketBuffer);
//             //std::tm timeValue = std::tm(decodeNtpMessage (ntpPacketBuffer));
//             //Logger::getInstance()->writeLn(decodeNtpMessage (ntpPacketBuffer));
//             //
//             // //getFirstSync (); // Set firstSync value if not set before
//             // Logger::getInstance()->writeLn("Sync frequency set low\n");
//             udp.stop ();
//             // setLastNTPSync (timeValue);
//             Logger::getInstance()->writeLn("Successful NTP sync at %s");
//
//             // if (onSyncEvent)
//             //     onSyncEvent (timeSyncd);
//             // return timeValue;
//         }
//
//         //ESP.wdtFeed ();
//         return true;
//     }
//     Logger::getInstance()->writeLn("-- No NTP Response :-(");
//     udp.stop ();
//     return false;
//   }


  static String getCurrentTime(){
    time_t tm = time(nullptr);
    return strtok(ctime(&tm), "\n");
  }

  static void configTZ(int tz){
    configTime(tz * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");

  }

};

#endif
