#include "TM1637_Colon.h"

#define CLK 2
#define DIO 3

TM1637_Colon tm1637(CLK, DIO);

void setup()
{
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL); // BRIGHT_TYPICAL = 2, BRIGHT_DARKEST = 0, BRIGHTEST = 7

  // LOVE, O=0
  tm1637.displayStr("L0UE");
  delay(2000);

  // loop
  tm1637.displayStr("HELL0-1234567890", 1000);
  delay(2000);

  // ON
  tm1637.displayStr("0N");
  delay(2000);

  // OFF
  tm1637.displayStr("0FF");
  delay(2000);

  // time
  tm1637.displayStr("23:34");
  delay(500);
  tm1637.displayStr("2334");
  delay(500);
  tm1637.displayStr("23:34");
  delay(500);
  tm1637.displayStr("2334");
  delay(1500);
}

void loop()
{
  tm1637.clearDisplay();
  delay(500);
  tm1637.displayStr("d0nE");
  delay(500);
}
