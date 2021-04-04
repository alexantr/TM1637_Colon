/*
 * TM1637_Colon.h
 * A library for the 4 digit display with dots and colon
 */

#ifndef TM1637_Colon_h
#define TM1637_Colon_h
#include <inttypes.h>
#include <Arduino.h>
/*************definitions for TM1637_Colon*********************/
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44

#define STARTADDR  0xc0
/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   0x80
#define POINT_OFF  0x00
/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7

class TM1637_Colon
{
  public:
    uint8_t Cmd_SetData;
    uint8_t Cmd_SetAddr;
    uint8_t Cmd_DispCtrl;
    boolean _PointFlag;     //_PointFlag=1:the clock point on
    TM1637_Colon(uint8_t, uint8_t);
    void init(void);        //To clear the display
    int  writeByte(int8_t wr_data);//write 8bit data to tm1637
    void start(void);//send start bits
    void stop(void); //send stop bits
    void display(int8_t DispData[],int8_t DispPointData[]);
    void display(uint8_t BitAddr,int8_t DispData,int8_t DispPointData);
    void colonOn(void);
    void colonOff(void);
    void clearDisplay(void);
    void set(uint8_t = BRIGHT_TYPICAL,uint8_t = 0x40,uint8_t = 0xc0);//To take effect the next time it displays.
    void coding(int8_t DispData[],int8_t DispPointData[]);
    int8_t coding(int8_t DispData,int8_t DispPointData);
    void bitDelay(void);
  private:
    uint8_t Clkpin;
    uint8_t Datapin;
};
#endif
