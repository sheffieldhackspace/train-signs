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
  : GFXcanvas1(BIG_CLOCK_WIDTH, BIG_CLOCK_HEIGHT), _board{board0, board1} {
  board0->setBuffer(buffer);
  board1->setBuffer(buffer);

  auto *keepalive = new uint8_t[]{keepalive0, keepalive1};
  xTaskCreate(keepaliveCallback, "keepalive", 4096, (void *) keepalive, 2, NULL);
}

void Adafruit_BigClock::display() {
  displayBoard(BOARD_TOP);
  displayBoard(BOARD_BOTTOM);
}

void Adafruit_BigClock::displayBoard(BOARD board) {
  pinMode(_board[board]->_dc, OUTPUT);
  digitalWrite(_board[board]->_dc, HIGH);

  for (int n = 0; n < 16; n++) {
    _board[board]->output_segment(false, n);
  }

  digitalWrite(_board[board]->_dc, LOW);
  digitalWrite(_board[board]->_dc, HIGH);

  for (int n = 0; n < 16; n++) {
    _board[board]->output_segment(true, n);
  }

  digitalWrite(_board[board]->_dc, LOW);
}

[[noreturn]] void Adafruit_BigClock::keepaliveCallback(void *arg) {
  const auto *keepalive = static_cast<uint8_t *>(arg);

  while (true) {
    pinMode(keepalive[0], OUTPUT);
    pinMode(keepalive[1], OUTPUT);

    digitalWrite(keepalive[0], HIGH);
    digitalWrite(keepalive[1], HIGH);
    delay(2);

    digitalWrite(keepalive[0], LOW);
    digitalWrite(keepalive[1], LOW);
    delay(2);
  }
}
