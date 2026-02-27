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

#include "DotText.h"

DotText::DotText(Adafruit_GFX *canvas, const String &text, int8_t align)
  : _canvas(canvas),
    _text(text),
    _align(align),
    _width(0),
    _height(0) {
  _canvas->getTextBounds(text, 0, 0, &_x, &_y, &_width, &_height);
}

int16_t DotText::calculateAlign(const int8_t a, const uint16_t b, const uint16_t d) {
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

void DotText::draw(const int16_t x, const int16_t y) const {
  int16_t begin = 0, end = 0;
  int16_t text_x = x - _x, text_y = y - _y;

  while (_text[end]) {
    if (_text[end] == '\n') {
      _canvas->println();

      text_y = _canvas->getCursorY();
      begin++;
    } else if (_text[end + 1] == '\n' || !_text[end + 1]) {
      String line = _text.substring(begin, end + 1);
      int16_t tx, ty;
      uint16_t tw, th;

      _canvas->getTextBounds(line, 0, 0, &tx, &ty, &tw, &th);
      _canvas->setCursor(text_x + calculateAlign(_align, _width, tw), text_y);

      _canvas->print(line);

      text_y = _canvas->getCursorY();
      begin = end + 1;
    }

    end++;
  }
}
