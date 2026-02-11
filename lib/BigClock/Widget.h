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

#ifndef TRAIN_SIGNS_WIDGET_H
#define TRAIN_SIGNS_WIDGET_H

#include <Org01Condensed.h>

#include "Adafruit_BigClock.h"

class Widget {
public:
  Widget(Adafruit_BigClock *canvas)
    : _canvas(canvas), _frame(0), _frames(1), _message(nullptr), _speed(5) {}

  void begin();
  void display();
  void waitForNextFrame();

  void setInvert(bool invert) {
    _canvas->invertDisplay(invert);
  }

  void setMessage(String *message) {
    _frame = 0;
    _message = message;
  }

  void setSpeed(uint8_t speed) { _speed = speed; }

private:
  Adafruit_BigClock *_canvas;
  uint16_t _frame;
  uint8_t _frames;

  String *_message;
  uint8_t _speed;
};


#endif //TRAIN_SIGNS_WIDGET_H
