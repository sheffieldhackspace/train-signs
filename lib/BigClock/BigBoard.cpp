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

BigBoard::BigBoard(int dc, int mosi, int sck)
  : _spi(new Adafruit_SPIDevice(-1, sck, -1, mosi, SPI_CLOCK_DIV2, SPI_BITORDER_MSBFIRST, SPI_MODE1)),
    _dc(dc), _buffer(0), _buffer_size(0) {
  _spi->begin();
}

void BigBoard::write(bool bit) {
  _buffer <<= 1;
  _buffer |= bit;
  _buffer_size++;

  if (_buffer_size >= 8) {
    _spi->transfer(_buffer);
    _buffer = 0;
    _buffer_size = 0;
  }
}
