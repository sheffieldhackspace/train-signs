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
    BigBoard(byte *fb, int board, int dc, int mosi, int sck);
    void output();

  private:
    void write(bool b);
    bool get_bit(int x, int y);
    void output_segment(bool odd_lines, int segment);

    SoftSPI *spi;
    short buffer_size;
    byte buffer;

    byte *_fb;
    int _board;
    int _dc;
    int _mosi;
    int _sck;
};

#endif
