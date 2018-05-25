//
// Read DHT11 to get temperature and humidity
// If reading fails set tt and hh to last read values and add 'X' to string
//
// Returns: String with temperature and humidity
//
#include "DHT.h"
#include "common.h"
#include "getDHT11.h"
#include "DebugMacros.h"

String getDHT11() {
  int tt, hh;
  static int old_tt = 25;
  static int old_hh = 50; // use these values if first read fails

  // only update T and H if successfully read from DHT11
  // otherwise read old values
  // if OK write new values
  String notok = "   X";
  tt = dht.readTemperature();
  hh = dht.readHumidity();
  if (isnan(tt) || isnan(hh) || tt > 100 || hh > 100) {
    DPRINTLN(F("Bad tt or hh"));
    DPRINT(tt); DPRINT(" "); DPRINTLN(hh);
    tt = old_tt;
    hh = old_hh;
  }
  else {
    DPRINTLN(F("DHT read OK"));
    notok = "";
    old_tt = tt;
    old_hh = hh;
  }
  String rth = String("T=" + String(tt) + "C   RH=" + String(hh) + "%" + notok);
  return rth;
}
