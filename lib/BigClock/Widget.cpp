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

#include "Widget.h"

void Widget::begin() {
  _canvas->begin();
  _canvas->setTextSize(1);
  _canvas->fillScreen(0);
  _canvas->setTextColor(1);
  _canvas->setFont(&Org01Condensed);
}

void Widget::display() {
  int16_t x, y, ax, ay;
  uint16_t w, h;

  _canvas->getTextBounds(*_message, 0, 0, &x, &y, &w, &h);
  getAlignmentOffset(w, h, &ax, &ay);

  _canvas->fillScreen(0);
  _canvas->setCursor(-x + ax, -y + ay);
  _canvas->println(*_message);
  _canvas->display();
}

void Widget::getAlignmentOffset(int16_t w, int16_t h, int16_t *ax, int16_t *ay) {
  int16_t yd = BC_HEIGHT - h;
  int16_t xd = BC_WIDTH - w;
  *ax = *ay = 0;

  if (yd > 0) {
    if (_vertical_align == MIDDLE) {
      *ay = yd / 2;
    } else if (_vertical_align == BOTTOM) {
      *ay = yd;
    }
  }

  if (xd > 0) {
    if (_text_align == CENTER) {
      *ax = xd / 2;
    } else if (_text_align == RIGHT) {
      *ax = xd;
    }
  }
}

void Widget::waitForNextFrame() {
  _frame++;

  if (_frame >= _frames) {
    _frame = 0;
  }

  delay(1000 / _speed);
}
