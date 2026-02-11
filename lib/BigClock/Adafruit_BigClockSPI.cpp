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

#include "Adafruit_BigClockSPI.h"

Adafruit_BigClockSPI::Adafruit_BigClockSPI(int8_t sck, int8_t mosi, int8_t latch, int8_t keepalive)
  : Adafruit_SPIDevice(-1, sck, -1, mosi, SPI_CLOCK_DIV2, SPI_BITORDER_MSBFIRST, SPI_MODE1),
    _latch(latch), _keepalive(keepalive), _buffer(0), _buffer_size(0) {}

bool Adafruit_BigClockSPI::begin() {
  if (_latch != -1) {
    pinMode(_latch, OUTPUT);
  }

  if (_keepalive != -1) {
    pinMode(_keepalive, OUTPUT);
  }

  return Adafruit_SPIDevice::begin();
}

void Adafruit_BigClockSPI::transfer(bool bit) {
  _buffer <<= 1;
  _buffer |= bit;
  _buffer_size++;

  if (_buffer_size >= 8) {
    Adafruit_SPIDevice::transfer(_buffer);
    _buffer = 0;
    _buffer_size = 0;
  }
}
