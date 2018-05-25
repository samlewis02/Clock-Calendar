//
// show7seg: display a seven segment number at given coordinates and given colour
// showColon: display a colon at given coordinates and given colour
// No check on coordinate values
//
// Parameters: x0:    x-coordinate
//             y0:    y-coordinate
//             num:   digit to display (0 - 9)
//             color: GxEPD_WHITE or GxEPD_BLACK
//
#include "Arduino.h"
#include "DHT.h"
#include "common.h"
#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w
#include "sevenSeg.h"

void hseg (uint16_t x0, uint16_t y0, uint16_t l, uint16_t color) {
  for (int i = 0, j = 3; i < 7; i++, j--) {
    display.drawLine(x0 + abs(j), y0 + i, x0 + l - abs(j), y0 + i, color);
  }
}

void vseg (uint16_t x0, uint16_t y0, uint16_t l, uint16_t color) {
  for (int i = 0, j = 3; i < 7; i++, j--) {
    display.drawLine(x0 + i, y0 + abs(j), x0 + i,  y0  + l - abs(j), color);
  }
}

void show7seg (uint16_t x0, uint16_t y0, byte num, uint16_t color) {
  if (num < 10) {
  byte sh1 = B10000000;
  if (segs[num] & (sh1 >>= 1) ) vseg(x0 + 42, y0 + 46, 40, color); // bottom right
  if (segs[num] & (sh1 >>= 1) ) vseg(x0,      y0 + 46, 40, color); // bottom left
  if (segs[num] & (sh1 >>= 1) ) hseg(x0 + 4,  y0 + 84, 40, color); // bottom
  if (segs[num] & (sh1 >>= 1) ) hseg(x0 + 4,  y0 + 42, 40, color); // middle
  if (segs[num] & (sh1 >>= 1) ) hseg(x0 + 4,  y0,      40, color); // top
  if (segs[num] & (sh1 >>= 1) ) vseg(x0 + 42, y0 + 4,  40, color); // top right
  if (segs[num] & (sh1 >>= 1) ) vseg(x0,      y0 + 4,  40, color); // top left
  }
}

void showColon (uint16_t x0, uint16_t y0, uint16_t color ) {
  vseg( x0 + 4, y0 + 10, 20, color);
  vseg( x0 + 4, y0 + 50, 20, color);
}
