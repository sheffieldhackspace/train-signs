/**
 * BigBoard.h - Driver for a board inside the BigClock
 */
#ifndef BigBoard_h
#define BigBoard_h

#include "Arduino.h"
#include "SoftSPI.h"

#define BOARD_TOP 1
#define BOARD_BOTTOM 0

#define MAX_X 12
#define MAX_Y 26

class BigBoard
{
  public:
    BigBoard(int board, int clock_pin, int latch_pin, int data_input_pin, int data_clock_pin);
    void output(byte *fb);

  private:
    void write(bool b);
    bool get_bit(byte *fb, int x, int y);
    void output_segment(byte *fb, bool odd_lines, int segment);

    SoftSPI *spi;
    short buffer_size;
    byte buffer;

    int board;
    int clock_pin;
    int latch_pin;
    int data_input_pin;
    int data_clock_pin;
};

#endif
