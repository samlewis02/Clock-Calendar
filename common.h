#ifndef _COMN_H
#define _COMN_H
const long MINUTE = (60 * 1000);
const long HOUR = (MINUTE * 60);
const long GCAL_TIME = (10 * MINUTE);
const long CURR_TIME = (12 * HOUR);
const long DHT_TIME = (10 * MINUTE);
const long NTP_TIME = (5 * MINUTE);
const int DHTPIN = D6; // Pin D6
const int DHTTYPE = DHT11;   // DHT 11
static const char* ssid = "P874-21-2-3F"; //  network SSID
static const char* pass  = "077190028";     // network password
const unsigned int localPort = 8888;  // local port to listen for UDP packets

static const char* GCALURL = "https://script.google.com/macros/s/AKfycbwY2YIhEJeJc3GbmubJ4diF-R8mYYCfEiHH49LnxS70AvGRPskt/exec";
static const char* host = "script.google.com";
const int httpsPort = 443;

static const char* CURRURL = "http://data.fixer.io/api/latest?access_key=0a371f7901a6260a0ea11865f1ad98da&symbols=TWD,GBP&format=1";

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
static byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
static const char ntpServerName[] = "3.tw.pool.ntp.org";
const int max_reattempts = 5;
const int timeZone = 8;     // HKT

byte const segs [] =
{
  B01110111, // 0
  B01000010, // 1
  B00111110, // 2
  B01011110, // 3
  B01001011, // 4
  B01011101, // 5
  B01111001, // 6
  B01000110, // 7
  B01111111, // 8
  B01001111  // 9
};
#endif
