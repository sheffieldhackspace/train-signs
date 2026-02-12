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

#ifndef ADAFRUIT_BIGCLOCKSPI_H
#define ADAFRUIT_BIGCLOCKSPI_H


#include <Adafruit_SPIDevice.h>

class Adafruit_BigClockSPI : public Adafruit_SPIDevice {
public:
  Adafruit_BigClockSPI(int8_t sck, int8_t mosi, int8_t latch, int8_t keepalive);
  bool begin();
  void transfer(bool b);
  void setKeepalive(bool b);
  void setLatch(bool b);

private:
  int8_t _keepalive;
  int8_t _latch;

  byte _buffer;
  int8_t _buffer_size;
};


#endif //ADAFRUIT_BIGCLOCKSPI_H
