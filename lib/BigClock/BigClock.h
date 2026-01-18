/*
  BigClock.h - Library for driving station BigClocks
*/
#ifndef BigClock_h
#define BigClock_h

#include "Arduino.h"
#include "SoftSPI.h"

#define MAX_X 12
#define MAX_Y 26

#define BOARD_TOP 1
#define BOARD_BOTTOM 0

#define BOARDSEL_PIN D2
#define LATCH_PIN D3
#define WOUT_PIN D4

class BigClock
{
  public:
    BigClock();
    void init();
    void output(byte *fb);
    static void sCallback(void *arg);
    
  private:
    void write_sbit(bool b);
    bool get_bit(byte *fb, int x, int y);
    void output_segment(byte *fb, int board, bool odd_lines, int segment);
    void output_board(byte *fb, int board);

    SoftSPI *spi;
    short bcount;
    byte by;
};

#endif
