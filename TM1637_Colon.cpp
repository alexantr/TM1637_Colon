/*
 * TM1637_Colon.cpp
 * A library for the 4 digit display with dots and colon
 */

#include "TM1637_Colon.h"
#include <Arduino.h>
static int8_t TubeTab[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x77,0x7c,
                           0x39,0x5e,0x79,0x71};//0~9,A,b,C,d,E,F
TM1637_Colon::TM1637_Colon(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}

void TM1637_Colon::init(void)
{
  clearDisplay();
}

int TM1637_Colon::writeByte(int8_t wr_data)
{
  uint8_t i,count1;
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
    wr_data >>= 1;
    digitalWrite(Clkpin,HIGH);

  }
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);
  pinMode(Datapin,INPUT);

  bitDelay();
  uint8_t ack = digitalRead(Datapin);
  if (ack == 0)
  {
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
  }
  bitDelay();
  pinMode(Datapin,OUTPUT);
  bitDelay();

  return ack;
}
//send start signal to TM1637
void TM1637_Colon::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1637
  digitalWrite(Datapin,HIGH);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,LOW);
}
//End of transmission
void TM1637_Colon::stop(void)
{
  digitalWrite(Clkpin,LOW);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,HIGH);
  digitalWrite(Datapin,HIGH);
}

void TM1637_Colon::display(int8_t DispData[],int8_t DispPointData[])
{
  int8_t SegData[4];
  int8_t SegPointData[4];
  uint8_t i;
  for(i=0;i<4;i++)
  {
    SegData[i] = DispData[i];
    SegPointData[i] = DispPointData[i];
  }
  coding(SegData, SegPointData);
  start();
  writeByte(ADDR_AUTO);
  stop();
  start();
  writeByte(Cmd_SetAddr);
  for(i=0;i<4;i++)
  {
    writeByte(SegData[i]);
  }
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}

void TM1637_Colon::display(uint8_t BitAddr,int8_t DispData,int8_t DispPointData)
{
  int8_t SegData;
  SegData = coding(DispData, DispPointData);
  start();
  writeByte(ADDR_FIXED);
  stop();
  start();
  writeByte(BitAddr|0xc0);
  writeByte(SegData);
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}

void TM1637_Colon::colonOn(void)
{
    display(0x04,0x00,0x80);
}

void TM1637_Colon::colonOff(void)
{
    display(0x04,0x00,0x00);
}

void TM1637_Colon::clearDisplay(void)
{
  display(0x00,0x7f,0x00);
  display(0x01,0x7f,0x00);
  display(0x02,0x7f,0x00);
  display(0x03,0x7f,0x00);
  display(0x04,0x00,0x00);
}

//To take effect the next time it displays.
void TM1637_Colon::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness; //Set the brightness and it takes effect the next time it displays.
}

void TM1637_Colon::coding(int8_t DispData[], int8_t DispPointData[])
{
  for(uint8_t i=0;i<4;i++)
  {
    if(DispData[i] == 0x7f) DispData[i] = 0x00 + DispPointData[i];
    else DispData[i] = TubeTab[DispData[i]] + DispPointData[i];
  }
}

int8_t TM1637_Colon::coding(int8_t DispData, int8_t DispPointData)
{
  if(DispData == 0x7f) DispData = 0x00 + DispPointData;//The bit digital tube off
  else DispData = TubeTab[DispData] + DispPointData;
  return DispData;
}

void TM1637_Colon::bitDelay(void)
{
  delayMicroseconds(50);
}
