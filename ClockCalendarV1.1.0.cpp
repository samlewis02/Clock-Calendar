/************************************************
 *
 * Clock calendar
 * Displays time, day and date, temperature and
 * humidity and current GBP->TWD exchange rate.
 *
 * Uses Waveshare 4.2 in display and DHT11 sensor
 * Uses threads to control timing of each reading
 *
 * Receives both currency info and calendar info in
 * JSON format.
 *
 * Uses Google CalendarGet2 script and fixwe.io for
 * currency. Currency is oly read once a day as
 * number of reads is limited to 1000/mth for the free
 * service
 *
 * Uses NTP server to supply time every 5 mins which is
 * stored and updated locally in case of read failure
 *
 * V1.1.0 Tested 9/5/18
 *
 */
 #include "Arduino.h"
#include <Wire.h>
#include "TimeLib.h"
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "DebugMacros.h"
#include <Thread.h>
#include <ThreadController.h>

// include library, include base class, make path known
#include <GxEPD.h>
// select the display class to use
#include <GxGDEW042T2/GxGDEW042T2.cpp>      // 4.2" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include "common.h"
#include "getCalendar.h"
#include "getCurrency.h"
#include "getDHT11.h"
#include "getNtpTime.h"
#include "parseJson.h"
#include "parseString.h"
#include "sevenSeg.h"
#include "updateClock.h"

GxIO_Class io(SPI, SS, 0, 2); //
GxEPD_Class display(io, D4, D0); // D4, D0

DHT dht(DHTPIN, DHTTYPE);

WiFiUDP Udp;
String rstring;
String currency;
String strth;

ThreadController ThreadControl = ThreadController();
Thread* gcalThread    = new Thread();
Thread* currThread    = new Thread();
Thread* ntpThread     = new Thread();
Thread* updateThread  = new Thread();
Thread* dht11Thread   = new Thread();

void handleUpdate (void)
{
  // wait for minute rollover to update
  if (second() <= 1) clockDisplay();
}


void handleNTP(void)
{
  int ut = getNtpTime();
  if (ut != 0) setTime(ut);
  DPRINT("Time = "); DPRINTLN(ut);
}

void handleGCAL(void)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    rstring = getCalendar();
    DPRINT("GCAL:"); DPRINTLN(rstring);

    DPRINT("[GCAL] Free heap = ");
    DPRINTLN(ESP.getFreeHeap());
  }
}

void handleCurr(void)
{
  String payload = getCurrency();
  DPRINT("Curr:"); DPRINTLN(payload);
  if (payload != "") {
    double conversion =  parse_json(payload);
    currency = String(F("1 GBP = ")) + conversion + (F(" NT$"));
  }
  else
    currency = "Could not get currency";
  DPRINTLN(currency);
  DPRINT("[Curr] Free heap = ");
  DPRINTLN(ESP.getFreeHeap());
}

void handleDHT11 (void)
{
  strth = getDHT11();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  // init the DHT11
  dht.begin();
  WiFi.begin(ssid, pass);
  display.init();
  initDisplay(); // set background
  while (WiFi.status() != WL_CONNECTED ) {
    yield();
    delay(500);
    Serial.print(".");
  }
  Udp.begin(localPort); // for NTP

  // initiate threads
  gcalThread->onRun(handleGCAL);    // Update Google Calendar data.
  gcalThread->setInterval(GCAL_TIME);  // 30 minute
  ThreadControl.add(gcalThread);
  currThread->onRun(handleCurr);    // Update currency data
  currThread->setInterval(CURR_TIME);   // 60 minute
  ThreadControl.add(currThread);
  ntpThread->onRun(handleNTP);    // Update Time
  ntpThread->setInterval(NTP_TIME);   // 5 minute
  ThreadControl.add(ntpThread);
  dht11Thread->onRun(handleDHT11);    // temperature and humidity
  dht11Thread->setInterval(DHT_TIME);   // 10 minutes
  ThreadControl.add(dht11Thread);
  updateThread->onRun(handleUpdate);    // Check for update display
  updateThread->setInterval(1000);   // 1 second
  ThreadControl.add(updateThread);

  // first time do straightaway
  handleGCAL();
  handleCurr();
  handleNTP();
  handleDHT11();
  handleUpdate();
}

void loop() {
  // put your main code here, to run repeatedly:
  ThreadControl.run();
}
