/*
  BigClock.h - Library for driving station BigClocks
*/
#ifndef BigClock_h
#define BigClock_h

#include "Arduino.h"
#include "SPI.h"
#if defined(__AVR__)
#include "TimerOne.h"
#elif defined(ESP8266)
#endif
#include <avr/pgmspace.h>

#define MAX_X 12
#define MAX_Y 26

#define BOARD_TOP 1
#define BOARD_BOTTOM 0

#define BOARDSEL_PIN 2 
#define LATCH_PIN 5
#define WOUT_PIN 6

class BigClock
{
  public:
    BigClock();
    void init();
    void output(byte *fb);
    static void sCallback();
    void callback();
    
  private:
    void write_sbit(bool b);
    bool get_bit(byte *fb, int x, int y);
    void output_segment(byte *fb, int board, bool odd_lines, int segment);
    void output_board(byte *fb, int board);

    short bcount;
    byte by;
};

#endif
