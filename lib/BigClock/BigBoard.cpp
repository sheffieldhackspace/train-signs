/*
 * BigBoard.h - Driver for a board inside the BigClock
 *
 * Copyright (c) 2013, Daniel Swann <hs@dswann.co.uk>
 * Copyright (c) 2026, Adam Kuczynski <github@typedef.io>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the owner nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "BigBoard.h"

BigBoard::BigBoard(byte *fb, int board, int dc, int mosi, int sck)
  : _fb(fb), _board(board), _dc(dc), _mosi(mosi), _sck(sck) {
  this->buffer = 0;
  this->buffer_size = 0;


  spi = new SoftSPI(this->_mosi, 0, this->_sck);
}

void BigBoard::write(bool bit) {
  buffer <<= 1;
  buffer |= bit;
  buffer_size++;

  if (buffer_size >= 8) {
    spi->transfer(buffer);
    buffer = 0;
    buffer_size = 0;
  }
}

bool BigBoard::get_bit(int x, int y) {
  return *(_fb + (y * MAX_X) + (x / 8)) >> (7 - (x % 8)) & 1;
}

void BigBoard::output_segment(bool even_row, int segment) {
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
    int offset_x = (segment * 6) + x;
    int offset_y = 12 + (2 * y) + (even_row ? 1 : 0);

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
    if (_board == BOARD_TOP) {
      offset_x = 95 - offset_x;
      offset_y = 25 - offset_y;
    }

    write(get_bit(offset_x, offset_y));

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

  write(even_row);
  write(false);
  write(false);
  write(false);
  write(false);
  write(false); // all white
  write(false);
  write(false);
}

void BigBoard::output() {
  spi->begin();
  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH);

  for (int n = 0; n < 16; n++) {
    output_segment(false, n);
  }

  digitalWrite(_dc, LOW);
  digitalWrite(_dc, HIGH);

  for (int n = 0; n < 16; n++) {
    output_segment(true, n);
  }

  digitalWrite(_dc, LOW);
  spi->end();
}

