// Update the Waveshare 4.2in display
//
// Assumes setIime() holds current time
// Global data as inputs:
// String rstring holds calendar info
// String strth holds temperature/humidity info
// double currency holds GBP to TWD conversion value
//
#include "Arduino.h"
// FreeFonts from Adafruit_GFX
#include "Timelib.h"
#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "DHTnew.h"
#include "common.h"
#include "updateClock.h"

//uncomment to display heap information
//#define SHOW_HEAP

void clockDisplay()
{
initDisplay();
// hour < 10 don't print digit in first location
if (hour() > 9) show7seg(20, 50, hour() / 10, GxEPD_WHITE);
show7seg(80, 50, hour() % 10, GxEPD_WHITE);
showColon(140, 50, GxEPD_WHITE);
if (minute() < 10) {
  show7seg(160, 50, 0, GxEPD_WHITE);
}
else {
  show7seg(160, 50, minute() / 10, GxEPD_WHITE);
}
show7seg(220, 50, minute() % 10, GxEPD_WHITE);

// *** Note: daySuffix is edit to DateStrings and TimeLib.h in Timelib library
String daynow = String(dayStr(weekday())) + " " + day() + daySuffix(day()) + " " + monthStr(month());
// centre string
//  daynow = "Wednesday 27th September"; // longest date string for test
if (daynow.length() < 24) {
  int indent = (24 - daynow.length()) / 2;
  for (int k = 0; k < indent; k++) {
    daynow = " " + daynow;
  }
}
// show day, date, month
display.setTextColor(GxEPD_BLACK);
display.setFont(&FreeMonoBold9pt7b);
display.setCursor(15, 180);
display.print(daynow);

// Show calendar info
display.setFont(&FreeSansBold9pt7b);
display.setCursor(0, 230);
display.print(parse_response(rstring));

#ifdef SHOW_HEAP
// show free heap
String freeheap = String("\n    Free heap = ") + ESP.getFreeHeap();
//display.setCursor(50,380);
display.print(freeheap);
#endif


// Show temp and humidity
display.setTextColor(GxEPD_WHITE);
display.setFont(&FreeMonoBold9pt7b);
display.setCursor(75, 30);
display.println(strth);

// show currency
display.setCursor(50, 380);
display.print(currency);
display.update();
}

// Set up background for display
void initDisplay() {
// digital display
display.setRotation(3);
display.fillScreen(GxEPD_BLACK);
// Box for day and date
display.fillRoundRect(10, 155, 280, 40, 10, GxEPD_WHITE);
// box for calendar info
display.fillRoundRect(10, 210, 280, 150, 10, GxEPD_WHITE);
}

String daySuffix (uint8_t day)
{
String ordinal_suffix;
switch (day) {
  case 1:
  case 21:
  case 31: ordinal_suffix = "st"; break;
  case 2:
  case 22: ordinal_suffix = "nd"; break;
  case 3:
  case 23: ordinal_suffix = "rd"; break;
  default: ordinal_suffix = "th";
};
return ordinal_suffix;
}
