/**
 * BigBoard.h - Driver for a board inside the BigClock
 */
#ifndef BigBoard_h
#define BigBoard_h

#include <Arduino.h>
#include <Adafruit_SPIDevice.h>

enum BOARD {
  BOARD_BOTTOM,
  BOARD_TOP,
};

class BigBoard {
public:
  BigBoard(byte *fb, BOARD board, int dc, int mosi, int sck);
  void output();

private:
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
