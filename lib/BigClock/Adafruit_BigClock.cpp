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

#include "Adafruit_BigClock.h"

Adafruit_BigClock::Adafruit_BigClock(BigBoard *board0, BigBoard *board1, uint8_t keepalive0, uint8_t keepalive1)
  : GFXcanvas1(BIG_CLOCK_WIDTH, BIG_CLOCK_HEIGHT), _keepalive0(keepalive0), _keepalive1(keepalive1) {
  board[BOARD_BOTTOM] = board0;
  board[BOARD_TOP] = board1;

  board0->setBuffer(buffer);
  board1->setBuffer(buffer);

  xTaskCreate(keepaliveCallback, "keepalive", 4096, NULL, 2, NULL);
}

void Adafruit_BigClock::display(void) {
  board[BOARD_TOP]->display();
  board[BOARD_BOTTOM]->display();
}

void Adafruit_BigClock::keepaliveCallback(void *arg) {
  while (true) {
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);

    digitalWrite(D7, HIGH);
    digitalWrite(D8, HIGH);
    delay(2);
    digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);
    delay(2);
  }
}
