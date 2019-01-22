#ifndef time_h
#define time_h

#include "../logger/Logger.h"
#include <ctime>
#include <string>
#include "Arduino.h"
#include <Wire.h>
#include <WiFiUdp.h>

#define DEFAULT_NTP_SERVER "192.168.1.141" // Default international NTP server. I recommend you to select a closer server to get better accuracy
#define DEFAULT_NTP_PORT 123 // Default local udp port. Select a different one if neccesary (usually not needed)
#define NTP_TIMEOUT 1500 // Response timeout for NTP requests
#define DEFAULT_NTP_TIMEZONE 0 // Select your local time offset. 0 if UTC time has to be used

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message

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
    if (! this->beginRTC()) {
      Logger::getInstance()->writeLn("Couldn't find RTC");
    }
    if (!this->isRunningRTC()) {
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
    time.tm_mday = bcd2bin(Wire.read()) - 1;
    time.tm_mon = bcd2bin(Wire.read()) - 1;
    time.tm_year = bcd2bin(Wire.read()) + 100;
    return time;
  }

  void setTimeRTC(std::tm *time){
    Logger::getInstance()->writeLn("Writing time to RTC");
    Wire.beginTransmission(0x68);
    Wire.write((byte)0);
    Wire.write(bin2bcd(time->tm_sec));
    Wire.write(bin2bcd(time->tm_min));
    Wire.write(bin2bcd(time->tm_hour));
    Wire.write(bin2bcd(time->tm_wday));
    Wire.write(bin2bcd(time->tm_mday) + 1);
    Wire.write(bin2bcd(time->tm_mon) + 1);
    Wire.write(bin2bcd(time->tm_year - 100));
    Wire.endTransmission();
  }

  boolean beginRTC(){
    Wire.begin();
    return true;
  }

  boolean isRunningRTC() {
    Wire.beginTransmission(0x68);
    Wire.write((byte)0);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 1);
    uint8_t ss = Wire.read();
    return !(ss>>7);
  }

  boolean GetNTP(String server) {
    Logger::getInstance()->writeLn("Synchronization with the NTP server started");
    WiFiUDP udp;
    udp.begin(123);

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

    byte ntpPacketBuffer[ NTP_PACKET_SIZE];
    uint32_t beginWait = millis ();
    while (millis () - beginWait < NTP_TIMEOUT) {
        if (udp.available() >= NTP_PACKET_SIZE) {
            Logger::getInstance()->writeLn("Receive NTP Response");
            udp.read(ntpPacketBuffer, 48);
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)ntpPacketBuffer[40] << 24;
            secsSince1900 |= (unsigned long)ntpPacketBuffer[41] << 16;
            secsSince1900 |= (unsigned long)ntpPacketBuffer[42] << 8;
            secsSince1900 |= (unsigned long)ntpPacketBuffer[43];

            time_t timeTemp = secsSince1900 - 2208988800UL;
            setTimeRTC(std::gmtime(&timeTemp));

            udp.stop ();
            return true;
        }
        ESP.wdtFeed ();
    }
    Logger::getInstance()->writeLn("Failed to find NTP time");
    udp.stop ();
    return false;
  }


  static String getCurrentTime(){
    time_t tm = time(nullptr);
    return strtok(ctime(&tm), "\n");
  }

  static void configTZ(int tz){
    configTime(tz * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");

  }

};

#endif
