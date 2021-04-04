#include "TM1637_Colon.h"

#define CLK 2
#define DIO 3

TM1637_Colon tm1637(CLK, DIO);

void setup()
{
  tm1637.init();
  tm1637.set(5); // BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}

void loop()
{
  int8_t ListDisp[4] = {0,1,2,3};
  int8_t ListDispPoint[4] = {POINT_ON,POINT_OFF,POINT_OFF,POINT_OFF};

  delay(150);

  while(1)
  {
    for(unsigned char BitSelect = 0; BitSelect < 4; BitSelect ++)
    {
      for(unsigned char BitSelect2 = 0; BitSelect2 < 4; BitSelect2 ++)
      {
        ListDispPoint[BitSelect2] = POINT_OFF;
      }
      ListDispPoint[BitSelect] = POINT_ON;

      tm1637.display(ListDisp, ListDispPoint);

      if (BitSelect == 0 || BitSelect == 1) tm1637.colonOn();
      else tm1637.colonOff();

      delay(250);
    }

    for(unsigned char BitSelect = 0; BitSelect < 4; BitSelect ++)
    {
      ListDisp[BitSelect] += 1;
      if (ListDisp[BitSelect] == 16) ListDisp[BitSelect] = 0;
    }
  }
}
