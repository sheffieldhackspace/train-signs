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

#ifndef TRAIN_SIGNS_ADAFRUIT_BIGCLOCK_H
#define TRAIN_SIGNS_ADAFRUIT_BIGCLOCK_H


#include <Adafruit_GFX.h>

#include "BigBoard.h"

#define BIG_CLOCK_WIDTH 96
#define BIG_CLOCK_HEIGHT 26

class Adafruit_BigClock : public GFXcanvas1 {
public:
  Adafruit_BigClock(BigBoard *board0, BigBoard *board1, uint8_t keepalive0, uint8_t keepalive1);
  void display(void);
  static void keepaliveCallback(void *arg);

private:
  BigBoard *board[2];
  uint8_t _keepalive0;
  uint8_t _keepalive1;
};


#endif //TRAIN_SIGNS_ADAFRUIT_BIGCLOCK_H
