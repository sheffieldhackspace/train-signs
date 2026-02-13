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

#include "Adafruit_Widget.h"

// a - alignment value
// b - boundary dimension (ie. width of the display)
// d - text dimension (ie. width of the text)
// return - text coordinate (ie. x position of the text)
int16_t Adafruit_Widget::getAlign(int8_t a, uint16_t b, uint16_t d) {
  int16_t distance = b - d;

  if (distance > 0) {
    if (a == 0) {
      return distance / 2;
    } else if (a == 1) {
      return distance;
    }
  }

  return 0;
}

// b - boundary dimension (ie. width of the display)
// d - text dimension (ie. width of the text)
// return - text coordinate (ie. x position of the text)
int16_t Adafruit_Widget::getScroll(uint16_t b, uint16_t d) {
  int16_t distance = d - b;

  if (distance > 0) {
    _frames = FRAMES_BEFORE + distance + FRAMES_AFTER;

    if (_frame < FRAMES_BEFORE) {
      return 0;
    } else if (_frame < FRAMES_BEFORE + distance) {
      return -(_frame - FRAMES_BEFORE);
    } else {
      return -distance;
    }
  }

  return 0;
}

void Adafruit_Widget::advanceFrame() {
  _frame++;

  if (_frame >= _frames) {
    _frame = 0;
  }

  delay(1000 / _speed);
}

void Adafruit_Widget::begin() {
  _canvas->begin();
  _canvas->setTextSize(1);
  _canvas->fillScreen(0);
  _canvas->setTextColor(1);
  _canvas->setFont(&Org_01);
}

void Adafruit_Widget::display() {
  int16_t x = 0, y = 0, text_x, text_y;
  uint16_t text_width, text_height;

  _canvas->fillScreen(0);
  _canvas->getTextBounds(*_text, 0, 0, &text_x, &text_y, &text_width, &text_height);

  flash();

  if (_text_wrap) {
    uint16_t widget_height = text_height;

    if (_image_height) {
      widget_height += _image_height + 2;
    }

    y += getScroll(BC_HEIGHT, widget_height);

    if (_image != nullptr) {
      x += getAlign(_horizontal_align, BC_WIDTH, _image_width);
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

    x += getScroll(BC_WIDTH, widget_width);
    x += getAlign(_horizontal_align, BC_WIDTH, widget_width);

    if (_image != nullptr && _horizontal_align != RIGHT) {
      _canvas->drawBitmap(x, 1, reinterpret_cast<uint8_t *>(_image), _image_width, _image_height, 1, 0);
      x += _image_width;
    }

    printText(x - text_x, y - text_y, text_width, text_height);
    x += text_width;

    if (_image != nullptr && _horizontal_align != LEFT) {
      _canvas->drawBitmap(x, 1, reinterpret_cast<uint8_t *>(_image), _image_width, _image_height, 1, 0);
    }
  }

  _canvas->display();
}

void Adafruit_Widget::flash() {
  if (_flash && _frame % (_speed / 2) == 0) {
    setInvert(!_invert);
  }
}

void Adafruit_Widget::printText(int16_t x, int16_t y, uint16_t w, uint16_t h) {
  int16_t begin = 0, end = 0;
  String s = *_text;

  y += getAlign(_vertical_align, BC_HEIGHT, h);

  while (s[end]) {
    if (s[end] == '\n') {
      _canvas->println();

      y = _canvas->getCursorY();
      begin++;
    } else if (s[end + 1] == '\n' || !s[end + 1]) {
      String line = s.substring(begin, end + 1);
      int16_t tx, ty;
      uint16_t tw, th;

      _canvas->getTextBounds(line, 0, 0, &tx, &ty, &tw, &th);
      _canvas->setCursor(x + getAlign(_horizontal_align, w, tw), y);

      _canvas->print(line);

      y = _canvas->getCursorY();
      begin = end + 1;
    }

    end++;
  }
}
