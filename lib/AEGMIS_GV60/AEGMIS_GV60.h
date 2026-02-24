/**
 * MIT License
 *
 * Copyright (c) 2026 Adam Kuczyński
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once


#include <Adafruit_GFX.h>

#include "AEGMIS_GV60_SPI.h"

#define BC_WIDTH (uint16_t) 96
#define BC_HEIGHT (uint16_t) 26

enum BOARD {
  BOARD_BOTTOM = 0,
  BOARD_TOP = 1,
};

class AEGMIS_GV60 : public GFXcanvas1 {
public:
  AEGMIS_GV60(AEGMIS_GV60_SPI *spi0, AEGMIS_GV60_SPI *spi1);
  void begin();
  void display();
  uint8_t getPixel(int16_t x, int16_t y);
  void invertDisplay(bool invert) override;

  [[noreturn]] static void keepaliveCallback(void *arg);

private:
  void displayBoard(BOARD board);
  void displaySegment(BOARD board, uint8_t segment, bool even_row);

  bool _invert;
  AEGMIS_GV60_SPI *_spi[2];
};

