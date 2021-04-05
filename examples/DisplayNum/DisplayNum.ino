#include "TM1637_Colon.h"

#define CLK 2
#define DIO 3

TM1637_Colon tm1637(CLK, DIO);

float number = -99.9;
int mult = 1;

void setup()
{
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL); // BRIGHT_TYPICAL = 2, BRIGHT_DARKEST = 0, BRIGHTEST = 7

  float num = -1.2345;

  tm1637.displayNum(num);
  delay(2000);

  tm1637.displayNum(num, 1);
  delay(2000);

  tm1637.displayNum(num, 2);
  delay(2000);

  tm1637.displayNum(num, 3);
  delay(2000);

  tm1637.displayNum(num, 4);
  delay(2000);
}

void loop()
{
  tm1637.displayNum(number);

  number += 5.42 * mult;
  if (abs(number) > 900) {
    mult *= -1;
  }

  delay(100);
}
