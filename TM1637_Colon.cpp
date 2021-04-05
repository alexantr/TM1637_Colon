/*
 * TM1637_Colon.cpp
 * A library for the 4 digit 14 pins common anode display with dots and colon
 * Connect pin 7 of display to pin 11 (GRID5) of TM1637
 * Connect pin 8 of display to pin 9 (SEG8) of TM1637
 */

#include "TM1637_Colon.h"
#include <Arduino.h>

//  --0x01--
// |        |
//0x20     0x02
// |        |
//  --0x40- -
// |        |
//0x10     0x04
// |        |
//  --0x08--

namespace {
  uint8_t char2segments(char c) {
    switch (c) {
      case '_' : return 0x08;
      case '^' : return 0x01; // ¯
      case '-' : return 0x40;
      case '*' : return 0x63; // °
      case ' ' : return 0x00; // space
      case 'A' : return 0x77; // upper case A
      case 'a' : return 0x5f; // lower case a
      case 'B' :              // lower case b
      case 'b' : return 0x7c; // lower case b
      case 'C' : return 0x39; // upper case C
      case 'c' : return 0x58; // lower case c
      case 'D' :              // lower case d
      case 'd' : return 0x5e; // lower case d
      case 'E' :              // upper case E
      case 'e' : return 0x79; // upper case E
      case 'F' :              // upper case F
      case 'f' : return 0x71; // upper case F
      case 'G' :              // upper case G
      case 'g' : return 0x35; // upper case G
      case 'H' : return 0x76; // upper case H
      case 'h' : return 0x74; // lower case h
      case 'I' : return 0x06; // 1
      case 'i' : return 0x04; // lower case i
      case 'J' : return 0x1e; // upper case J
      case 'j' : return 0x16; // lower case j
      case 'K' :              // upper case K
      case 'k' : return 0x75; // upper case K
      case 'L' :              // upper case L
      case 'l' : return 0x38; // upper case L
      case 'M' :              // twice tall n
      case 'm' : return 0x37; // twice tall ∩
      case 'N' :              // lower case n
      case 'n' : return 0x54; // lower case n
      case 'O' :              // lower case o
      case 'o' : return 0x5c; // lower case o
      case 'P' :              // upper case P
      case 'p' : return 0x73; // upper case P
      case 'Q' : return 0x7b; // upper case Q
      case 'q' : return 0x67; // lower case q
      case 'R' :              // lower case r
      case 'r' : return 0x50; // lower case r
      case 'S' :              // 5
      case 's' : return 0x6d; // 5
      case 'T' :              // lower case t
      case 't' : return 0x78; // lower case t
      case 'U' :              // lower case u
      case 'u' : return 0x1c; // lower case u
      case 'V' :              // twice tall u
      case 'v' : return 0x3e; // twice tall u
      case 'W' : return 0x7e; // upside down A
      case 'w' : return 0x2a; // separated w
      case 'X' :              // upper case H
      case 'x' : return 0x76; // upper case H
      case 'Y' :              // lower case y
      case 'y' : return 0x6e; // lower case y
      case 'Z' :              // separated Z
      case 'z' : return 0x1b; // separated Z
    }
    return 0;
  }
}

static int8_t tube_tab[] = {0x3f, 0x06, 0x5b, 0x4f,
                            0x66, 0x6d, 0x7d, 0x07,
                            0x7f, 0x6f, 0x77, 0x7c,
                            0x39, 0x5e, 0x79, 0x71
                           }; //0~9,A,b,C,d,E,F

TM1637_Colon::TM1637_Colon(uint8_t clk, uint8_t data)
{
  clkpin = clk;
  datapin = data;
  pinMode(clkpin, OUTPUT);
  pinMode(datapin, OUTPUT);
}

void TM1637_Colon::init(void)
{
  clearDisplay();
}

int TM1637_Colon::writeByte(int8_t wr_data)
{
  for (uint8_t i = 0; i < 8; i++) { // Sent 8bit data
    digitalWrite(clkpin, LOW);

    if (wr_data & 0x01) {
      digitalWrite(datapin, HIGH); // LSB first
    } else {
      digitalWrite(datapin, LOW);
    }

    wr_data >>= 1;
    digitalWrite(clkpin, HIGH);
  }

  digitalWrite(clkpin, LOW); // Wait for the ACK
  digitalWrite(datapin, HIGH);
  digitalWrite(clkpin, HIGH);
  pinMode(datapin, INPUT);

  bitDelay();
  uint8_t ack = digitalRead(datapin);

  if (ack == 0) {
    pinMode(datapin, OUTPUT);
    digitalWrite(datapin, LOW);
  }

  bitDelay();
  pinMode(datapin, OUTPUT);
  bitDelay();

  return ack;
}

// Send start signal to TM1637 (start = when both pins goes low)
void TM1637_Colon::start(void)
{
  digitalWrite(clkpin, HIGH);
  digitalWrite(datapin, HIGH);
  digitalWrite(datapin, LOW);
  digitalWrite(clkpin, LOW);
}

// End of transmission (stop = when both pins goes high)
void TM1637_Colon::stop(void)
{
  digitalWrite(clkpin, LOW);
  digitalWrite(datapin, LOW);
  digitalWrite(clkpin, HIGH);
  digitalWrite(datapin, HIGH);
}

void TM1637_Colon::display(int8_t disp_data[])
{
  int8_t seg_data[4];
  int8_t seg_point_data[4] = {POINT_OFF, POINT_OFF, POINT_OFF, POINT_OFF};
  uint8_t i;

  for (i = 0; i < 4; i++) {
    seg_data[i] = disp_data[i];
  }

  coding(seg_data, seg_point_data);
  start();              // Start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO); // Command1: Set data
  stop();
  start();
  writeByte(cmd_set_addr); // Command2: Set address (automatic address adding)

  for (i = 0; i < 4; i++) {
    writeByte(seg_data[i]); // Transfer display data (8 bits x num_of_digits)
  }

  stop();
  start();
  writeByte(cmd_disp_ctrl); // Control display
  stop();
}

void TM1637_Colon::display(int8_t disp_data[], int8_t disp_point_data[])
{
  int8_t seg_data[4];
  int8_t seg_point_data[4];
  uint8_t i;

  for (i = 0; i < 4; i++) {
    seg_data[i] = disp_data[i];
    seg_point_data[i] = disp_point_data[i];
  }

  coding(seg_data, seg_point_data);
  start();              // Start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO); // Command1: Set data
  stop();
  start();
  writeByte(cmd_set_addr); // Command2: Set address (automatic address adding)

  for (i = 0; i < 4; i++) {
    writeByte(seg_data[i]); // Transfer display data (8 bits x num_of_digits)
  }

  stop();
  start();
  writeByte(cmd_disp_ctrl); // Control display
  stop();
}

void TM1637_Colon::display(uint8_t bit_addr, int8_t disp_data, int8_t disp_point_data)
{
  int8_t seg_data;

  seg_data = coding(disp_data, disp_point_data);
  start();               // Start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED); // Command1: Set data
  stop();
  start();
  writeByte(bit_addr | 0xc0); // Command2: Set data (fixed address)
  writeByte(seg_data);        // Transfer display data 8 bits
  stop();
  start();
  writeByte(cmd_disp_ctrl); // Control display
  stop();
}

//--------------------------------------------------------

void TM1637_Colon::displayError(void)
{
  int8_t tempListDisp[4] = {'-', '-', '-', '-'};
  int8_t tempListDispPoint[4] = {POINT_OFF, POINT_OFF, POINT_OFF, POINT_OFF};
  display(tempListDisp, tempListDispPoint);
}

void TM1637_Colon::displayNum(float num, int decimal, uint16_t loop_delay)
{
  int number = round(fabs(num) * pow(10, decimal));

  unsigned int number_of_digits = String(number).length();
  if (decimal > 0) {
    number_of_digits += 1;
  }
  if (num < 0) {
    number_of_digits += 1;
  }

  char str[number_of_digits];
  dtostrf(num, number_of_digits, decimal, str);

  // pad with spaces
  int i, min_limit = decimal > 0 ? 5: 4;
  if (number_of_digits < min_limit) {
    int amount = min_limit - number_of_digits;
    if (decimal > 0) {
      char new_str[] = "     ";
      for (i = 0; i < number_of_digits; i++) {
        new_str[amount++] = str[i];
      }
      displayStr(new_str, loop_delay);
    } else {
      char new_str[] = "    ";
      for (i = 0; i < number_of_digits; i++) {
        new_str[amount++] = str[i];
      }
      displayStr(new_str, loop_delay);
    }
  } else {
    displayStr(str, loop_delay);
  }
}

void TM1637_Colon::displayStr(char str[], uint16_t loop_delay)
{
  int i, len = strlen(str);
  bool show_colon = false;

  // Find str length without dots
  int new_len = 0;
  for (i = 0; i < len; i++) {
    if (String(str[i]) == ":") {
      if (new_len == 2) {
        show_colon = true;
      }
    } else if (String(str[i]) != ".") {
      new_len++;
    } else if (i == 0) {
      new_len++;
    }
  }

  if (new_len == 0) {
    clearDisplay();
  } else {

    char new_str[new_len];
    int8_t dots[new_len];
    // Fill dots with POINT_OFF
    for (i = 0; i < new_len; i++) {
      dots[i] = POINT_OFF;
    }

    int curr_char_pos = 0;
    int prev_char_pos = 0;
    for (i = 0; i < len; i++) {
      if (String(str[i]) == ".") {
        // Add space before first dot
        if (i == 0) {
          new_str[curr_char_pos] = ' ';
          prev_char_pos = curr_char_pos;
          curr_char_pos++;
        }
        dots[prev_char_pos] = POINT_ON;
      } else if (String(str[i]) != ":") {
        new_str[curr_char_pos] = str[i];
        prev_char_pos = curr_char_pos;
        curr_char_pos++;
      }
    }

    if (new_len <= 4) {
      for (i = 0; i < 4; i++) {
        if (i >= new_len) {
          display(i, 0x7f, POINT_OFF);
        } else {
          display(i, new_str[i], dots[i]);
        }
      }
      if (show_colon) {
        colonOn();
      } else {
        colonOff();
      }
    } else {
      colonOff();

      int offset =- 4;

      for (i = 0; i <= new_len + 4; i++) {
        for (int j = offset, k = 0; j < 4 + offset; j++, k++) {
          if (j < 0 || j >= new_len) {
            display(k, 0x7f, POINT_OFF);
          } else {
            display(k, new_str[j], dots[j]);
          }
        }
        offset++;
        delay(loop_delay);
      }
    }
  }
}

void TM1637_Colon::clearDisplay(void)
{
  display(0x00, 0x7f, POINT_OFF);
  display(0x01, 0x7f, POINT_OFF);
  display(0x02, 0x7f, POINT_OFF);
  display(0x03, 0x7f, POINT_OFF);
  display(0x04, 0x7f, POINT_OFF);
}

// To take effect the next time it displays.
void TM1637_Colon::set(uint8_t brightness, uint8_t set_data, uint8_t set_addr)
{
  cmd_set_data = set_data;
  cmd_set_addr = set_addr;
  //Set the brightness and it takes effect the next time it displays.
  cmd_disp_ctrl = 0x88 + brightness;
}

// Light the clock point ":".
// To take effect the next time it displays.
void TM1637_Colon::colonOn(void)
{
  display(0x04, 0x00, POINT_ON);
}

// Turn off the clock point ":".
// To take effect the next time it displays.
void TM1637_Colon::colonOff(void)
{
  display(0x04, 0x00, POINT_OFF);
}

void TM1637_Colon::coding(int8_t disp_data[], int8_t disp_point_data[])
{
  for (uint8_t i = 0; i < 4; i++) {
    disp_data[i] = coding(disp_data[i], disp_point_data[i]);
  }
}

int8_t TM1637_Colon::coding(int8_t disp_data, int8_t disp_point_data)
{
  if (disp_data == 0x7f) {
    disp_data = 0x00; // Clear digit
  } else if (disp_data >= 0 && disp_data < int(sizeof(tube_tab) / sizeof(*tube_tab))) {
    disp_data = tube_tab[disp_data];
  } else if (disp_data >= '0' && disp_data <= '9') {
    disp_data = tube_tab[int(disp_data) - 48]; // char to int (char "0" = ASCII 48)
  } else {
    disp_data = char2segments(disp_data);
  }
  disp_data += disp_point_data;

  return disp_data;
}

void TM1637_Colon::bitDelay(void)
{
  delayMicroseconds(50);
}
