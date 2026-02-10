/**
 * BigBoard.h - Driver for a board inside the BigClock
 */
#ifndef BigBoard_h
#define BigBoard_h

#include <Arduino.h>
#include <Adafruit_SPIDevice.h>

class BigBoard {
public:
  BigBoard(int dc, int mosi, int sck);
  void write(bool b);

  Adafruit_SPIDevice *_spi;
  short _buffer_size;
  byte _buffer;

  int _dc;
};

#endif
