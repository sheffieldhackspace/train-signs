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

#include <Fonts/Org_01.h>

#include "DotWidget.h"

// a - alignment value
// b - boundary dimension (i.e. width of the display)
// d - text dimension (i.e. width of the text)
// return - text coordinate (i.e. x position of the text)
int16_t DotWidget::getAlign(const int8_t a, const uint16_t b, const uint16_t d) {
  auto result = 0;

  if (const auto distance = b - d; distance > 0) {
    if (a == 0) {
      result = distance / 2;
    } else if (a == 1) {
      result = distance;
    }
  }

  return static_cast<int16_t>(result);
}

// b - boundary dimension (i.e. width of the display)
// d - text dimension (i.e. width of the text)
// return - text coordinate (i.e. x position of the text)
int16_t DotWidget::getScroll(const uint16_t b, const uint16_t d) {
  auto result = 0;

  if (const auto distance = d - b; distance > 0) {
    _frames = FRAMES_BEFORE + distance + FRAMES_AFTER;

    if (_frame < FRAMES_BEFORE) {
      result = 0;
    } else if (_frame < FRAMES_BEFORE + distance) {
      result = -(_frame - FRAMES_BEFORE);
    } else {
      result = -distance;
    }
  }

  return static_cast<int16_t>(result);
}

void DotWidget::advanceFrame() {
  _frame++;

  if (_frame >= _frames) {
    _frame = 0;
  }

  delay(1000 / _speed);
}

void DotWidget::begin() const {
  _canvas->setTextSize(1);
  _canvas->fillScreen(0);
  _canvas->setTextColor(1);
  _canvas->setFont(&Org_01);
}

void DotWidget::flash() {
  if (_flashing && _frame % (_speed / 2) == 0) {
    setInvert(!_invert);
  }
}

void DotWidget::print() {
  int16_t x = 0, y = 0, text_x, text_y;
  uint16_t text_width, text_height;

  _canvas->fillScreen(0);
  _canvas->getTextBounds(_text, 0, 0, &text_x, &text_y, &text_width, &text_height);

  flash();

  if (_text_wrap) {
    uint16_t widget_height = text_height;

    if (_image_height) {
      widget_height += _image_height + 2;
    }

    y += getScroll(_canvas->height(), widget_height);

    if (_image != nullptr) {
      x += getAlign(_horizontal_align, _canvas->width(), _image_width);

      _canvas->drawBitmap(x, y, reinterpret_cast<uint8_t *>(_image), _image_width, _image_height, 1, 0);

      x = 0;
      y += _image_height + 2;
    }

    printText(x - text_x, y - text_y, text_width, text_height);
  } else {
    uint16_t widget_width = text_width + _image_width;

    if (_horizontal_align == CENTER) {
      widget_width += _image_width;
    }

    x += getScroll(_canvas->width(), widget_width);
    x += getAlign(_horizontal_align, _canvas->width(), widget_width);

    if (_image != nullptr && _horizontal_align != RIGHT) {
      y = getAlign(_vertical_align, _canvas->height(), _image_height);

      _canvas->drawBitmap(x, y, reinterpret_cast<uint8_t *>(_image), _image_width, _image_height, 1, 0);

      x += _image_width;
      y = 0;
    }

    printText(x - text_x, y - text_y, text_width, text_height);
    x += text_width;

    if (_image != nullptr && _horizontal_align != LEFT) {
      y = getAlign(_vertical_align, _canvas->height(), _image_height);

      _canvas->drawBitmap(x, y, reinterpret_cast<uint8_t *>(_image), _image_width, _image_height, 1, 0);
    }
  }
}

void DotWidget::printText(const int16_t x, const int16_t y, const uint16_t w, const uint16_t h) const {
  int16_t begin = 0, end = 0, y1 = y;
  String s = _text;

  y1 += getAlign(_vertical_align, _canvas->height(), h);

  while (s[end]) {
    if (s[end] == '\n') {
      _canvas->println();

      y1 = _canvas->getCursorY();
      begin++;
    } else if (s[end + 1] == '\n' || !s[end + 1]) {
      String line = s.substring(begin, end + 1);
      int16_t tx, ty;
      uint16_t tw, th;

      _canvas->getTextBounds(line, 0, 0, &tx, &ty, &tw, &th);
      _canvas->setCursor(x + getAlign(_horizontal_align, w, tw), y1);

      _canvas->print(line);

      y1 = _canvas->getCursorY();
      begin = end + 1;
    }

    end++;
  }
}
