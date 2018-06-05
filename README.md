# Clock-Calendar
Uses Waveshare 4.2in with Wemos D1 mini to show date, time, temperature,
humidity, calendar and currency.

Calendar information is read from Google Calendar. A script is included which should
be copied into script.google.com. When it is published you will need to copy the key
into the definition of GCALURL. The script returns calendar information for today and
tomorrow in Json format. This is parsed and the two days are displayed atlternately
each minute. The calendar info is updated every 10 minutes.

Currency information is read from the fixer.io website. At present the conversion is
GBP to TWD but this is easily changed (see below).

Time is read from an NTP server. TimeLib library is used to convert to day of the week,
day, month, hour and minute. The Waveshare display is slow so we only update once a minute.

Temperature and humidity are read from a DHT11. I have found readings to be a little
erratic so if I do not get a sensible reading I use the previous values.

Timing is controlled using Threads. There is a thread for: reading calendar; reading currency;
reading temperature and humidity; chhecking time; and updating the display.

The following libraries are required:
GxEPD
DHT
Adafruit_GFX
Threads
ESP8266HTTPClient
ESP8266Wifi
WifiUdp

Wiring from Wemos D1 mini to Waveshare display:
BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

