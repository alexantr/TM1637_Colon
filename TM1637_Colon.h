/*
 * TM1637_Colon.h
 * A library for the 4 digit 14 pins common anode display with dots and colon
 * Connect pin 7 of display to pin 11 (GRID5) of TM1637
 * Connect pin 8 of display to pin 9 (SEG8) of TM1637
 */

#ifndef TM1637_Colon_h
#define TM1637_Colon_h
#include <inttypes.h>
#include <Arduino.h>
/*******************Definitions for TM1637*********************/
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44

#define STARTADDR  0xc0
/******Definitions for the dot point of the digit tube ********/
#define POINT_ON   0x80
#define POINT_OFF  0x00
/**************Definitions for brightness**********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7

class TM1637_Colon
{
  public:
    uint8_t cmd_set_data;
    uint8_t cmd_set_addr;
    uint8_t cmd_disp_ctrl;
    TM1637_Colon(uint8_t, uint8_t);
    void init(void);               // To clear the display
    int writeByte(int8_t wr_data); // Write 8bit data to tm1637
    void start(void);              // Send start bits
    void stop(void);               // Send stop bits
    void display(int8_t DispData[]);
    void display(int8_t DispData[], int8_t DispPointData[]);
    void display(uint8_t BitAddr, int8_t DispData, int8_t DispPointData = 0x00);
    void displayError(void);
    void displayNum(float num, int decimal = 0, uint16_t loop_delay = 500);
    void displayStr(char str[], uint16_t loop_delay = 500);
    void clearDisplay(void);
    void set(uint8_t = BRIGHT_TYPICAL, uint8_t = 0x40, uint8_t = 0xc0); //To take effect the next time it displays.
    void colonOn(void);
    void colonOff(void);
    void coding(int8_t DispData[], int8_t DispPointData[]);
    int8_t coding(int8_t DispData, int8_t DispPointData = 0x00);
    void bitDelay(void);
  private:
    uint8_t clkpin;
    uint8_t datapin;
};
#endif
