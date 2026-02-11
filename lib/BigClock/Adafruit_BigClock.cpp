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

Adafruit_BigClock::Adafruit_BigClock(Adafruit_BigClockSPI *board0, Adafruit_BigClockSPI *board1)
  : GFXcanvas1(BIG_CLOCK_WIDTH, BIG_CLOCK_HEIGHT), _board{board0, board1} {}

void Adafruit_BigClock::begin() {
  _board[BOARD_TOP]->begin();
  _board[BOARD_BOTTOM]->begin();

  xTaskCreate(keepaliveCallback, "keepalive", 4096, (void *) _board, 2, NULL);
}

void Adafruit_BigClock::display() {
  displayBoard(BOARD_TOP);
  displayBoard(BOARD_BOTTOM);
}

void Adafruit_BigClock::displayBoard(BOARD board) {
  digitalWrite(_board[board]->_latch, HIGH);

  for (uint8_t n = 0; n < 16; n++) {
    displaySegment(board, n, false);
  }

  digitalWrite(_board[board]->_latch, LOW);
  digitalWrite(_board[board]->_latch, HIGH);

  for (uint8_t n = 0; n < 16; n++) {
    displaySegment(board, n, true);
  }

  digitalWrite(_board[board]->_latch, LOW);
}

void Adafruit_BigClock::displaySegment(BOARD board, uint8_t segment, bool even_row) {
  // Odd segments start in the first column and second row of the current segment
  // Even segments start in the last column and last row of the previous segment
  bool even_segment = segment % 2;
  int x = even_segment ? -1 : 0;
  int y = even_segment ? 6 : 1;

  // Even segments have 40 pixels
  // Odd segments have 38 pixels, but still requires 40 bits (5 bytes) of data
  for (int i = 0; i < 40; i++) {
    // Display origin point is in the upper left corner
    // Coordinates are growing bottom right
    int16_t offset_x = (segment * 6) + x;
    int16_t offset_y = 12 + (2 * y) + (even_row ? 1 : 0);

    // To whoever designed this part:
    // You know what you did, I hope your partner steals your duvet every night.

    // Long story short:
    // Even rows start with a column of 6 pixels: 0,1,2,3,4,5
    // Odd rows start with a column of 7 pixels: 1,2,3,4,5,6,0
    //
    // It is significantly easier to count the odd rows as still having 6 pixels
    // and then take the first pixel of the next column that is "out of bounds"
    // and manually move it to the "pixel 0" of the previous column.
    if (offset_y == 12) {
      offset_x -= 1;
      offset_y = 13;
    }

    // First segment of the bottom board starts in the middle left of the display
    // Segments travel to the bottom right of the display
    // First segment of the top board starts in the middle right of the display
    // Segments travel to the top left of the display
    if (board == BOARD_TOP) {
      offset_x = 95 - offset_x;
      offset_y = 25 - offset_y;
    }

    _board[board]->transfer(getPixel(offset_x, offset_y));

    // Odd segments contain 38 pixels
    // Even rows have columns of 6,7,6,7,6,6 pixels
    // Odd rows have columns of 7,6,7,6,7,5 pixels
    // Even segments contain 40 pixels
    // Even rows have colums of 1,6,7,6,7,6,7 pixels
    // Odd rows have colums of 1,7,6,7,6,7,6 pixels
    if (y < 6) {
      y++;
    } else {
      x++;
      y = (x % 2) ? 0 : 1;
    }
  }

  _board[board]->transfer(even_row);
  _board[board]->transfer(false);
  _board[board]->transfer(false);
  _board[board]->transfer(false);
  _board[board]->transfer(false);
  _board[board]->transfer(false); // all white
  _board[board]->transfer(false);
  _board[board]->transfer(false);
}

[[noreturn]] void Adafruit_BigClock::keepaliveCallback(void *arg) {
  Adafruit_BigClockSPI **keepalive = static_cast<Adafruit_BigClockSPI **>(arg);

  while (true) {
    digitalWrite(keepalive[0]->_keepalive, HIGH);
    digitalWrite(keepalive[1]->_keepalive, HIGH);
    delay(2);

    digitalWrite(keepalive[0]->_keepalive, LOW);
    digitalWrite(keepalive[1]->_keepalive, LOW);
    delay(2);
  }
}
