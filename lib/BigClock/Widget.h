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

#include "Adafruit_BigClock.h"

#define FRAMES_BEFORE 20
#define FRAMES_AFTER 20

enum TEXT_ALIGN {
  LEFT = -1,
  CENTER = 0,
  RIGHT = 1,
};

enum VERTICAL_ALIGN {
  TOP = -1,
  MIDDLE = 0,
  BOTTOM = 1,
};

class Widget {
public:
  Widget(Adafruit_BigClock *canvas)
    : _canvas(canvas), _frame(0), _frames(FRAMES_BEFORE + FRAMES_AFTER),
      _flash(false), _message(nullptr), _speed(5), _text_align(LEFT), _text_wrap(false), _vertical_align(TOP) {}

  void applyAlign(int8_t a, int16_t b, int16_t d, int16_t *c);
  void applyFlash();
  void applyScroll(int16_t b, int16_t d, int16_t *c);
  void begin();
  void display();
  void printMessage(int16_t x, int16_t y, uint16_t w);
  void waitForNextFrame();

  void setFlash(bool flash) {
    _flash = flash;
  }

  void setInvert(bool invert) {
    _invert = invert;
    _canvas->invertDisplay(invert);
  }

  void setMessage(String *message) {
    _frame = 0;
    _frames = FRAMES_BEFORE + FRAMES_AFTER;
    _message = message;
  }

  void setSpeed(uint8_t speed) {
    _speed = speed;
  }

  void setTextAlign(TEXT_ALIGN align) {
    _text_align = align;
  }

  void setTextWrap(bool wrap) {
    _text_wrap = wrap;
    _canvas->setTextWrap(wrap);
  }

  void setVerticalAlign(VERTICAL_ALIGN align) {
    _vertical_align = align;
  }

private:
  Adafruit_BigClock *_canvas;
  uint16_t _frame;
  uint16_t _frames;

  bool _flash;
  bool _invert;
  String *_message;
  uint8_t _speed;
  TEXT_ALIGN _text_align;
  bool _text_wrap;
  VERTICAL_ALIGN _vertical_align;
};


#endif //TRAIN_SIGNS_WIDGET_H
