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
#include <Fonts/Org_01.h>

void Widget::begin() {
  _canvas->begin();
  _canvas->setTextSize(1);
  _canvas->fillScreen(0);
  _canvas->setTextColor(1);
  _canvas->setFont(&Org_01);
}

void Widget::applyHorizontalAlign(int16_t w, int16_t *x) {
  int16_t xd = BC_WIDTH - w;

  if (xd > 0) {
    if (_text_align == CENTER) {
      *x += xd / 2;
    } else if (_text_align == RIGHT) {
      *x += xd;
    }
  }
}

void Widget::applyVerticalAlign(int16_t h, int16_t *y) {
  int16_t yd = BC_HEIGHT - h;

  if (yd > 0) {
    if (_vertical_align == MIDDLE) {
      *y += yd / 2;
    } else if (_vertical_align == BOTTOM) {
      *y += yd;
    }
  }
}

// b - boundary dimension (ie. width of the display)
// d - text dimension (ie. width of the text)
// c - text coordinate (ie. x position of the text)
void Widget::applyScroll(int16_t b, int16_t d, int16_t *c) {
  int16_t distance = d - b;

  if (distance > 0) {
    _frames = distance + 40;

    if (_frame >= 20 && _frame - 20 < distance) {
      *c += -(_frame - 20);
    } else if (_frame - 20 >= distance) {
      *c += -distance;
    }
  }
}

void Widget::display() {
  int16_t x, y, tx, ty;
  uint16_t tw, th;

  _canvas->getTextBounds(*_message, 0, 0, &tx, &ty, &tw, &th);
  x = -tx;
  y = -ty;

  if (_text_wrap) {
    applyScroll(BC_HEIGHT, th, &y);
  } else {
    applyScroll(BC_WIDTH, th, &x);
  }

  applyVerticalAlign(th, &y);

  _canvas->fillScreen(0);
  this->printMessage(x, y);
  _canvas->display();
}

void Widget::printMessage(int16_t x, int16_t y) {
  int16_t begin = 0, end = 0;
  String s = *_message;

  while (s[end]) {
    if (s[end] == '\n') {
      _canvas->println();

      y = _canvas->getCursorY();
      begin++;
    } else if (s[end + 1] == '\n' || !s[end + 1]) {
      String line = s.substring(begin, end + 1);
      int16_t x1 = x, bx, by;
      uint16_t bw, bh;

      _canvas->getTextBounds(line, x, y, &bx, &by, &bw, &bh);
      applyHorizontalAlign(bw, &x1);

      _canvas->setCursor(x1, y);
      _canvas->print(line);

      y = _canvas->getCursorY();
      begin = end + 1;
    }

    end++;
  }
}

void Widget::waitForNextFrame() {
  _frame++;

  if (_frame >= _frames) {
    _frame = 0;
  }

  delay(1000 / _speed);
}
