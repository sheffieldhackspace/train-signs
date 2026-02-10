/**
 * BigBoard.h - Driver for a board inside the BigClock
 */
#ifndef BigBoard_h
#define BigBoard_h

#include <Arduino.h>
#include <Adafruit_SPIDevice.h>

enum BOARD {
  BOARD_BOTTOM = 0,
  BOARD_TOP = 1,
};

class BigBoard {
public:
  BigBoard(BOARD board, int dc, int mosi, int sck);
  void setBuffer(byte *fb);
  void write(bool b);
  bool get_bit(int x, int y);
  void output_segment(bool odd_lines, int segment);

  Adafruit_SPIDevice *_spi;
  short _buffer_size;
  byte _buffer;

  byte *_fb;
  BOARD _board;
  int _dc;
};

#endif
