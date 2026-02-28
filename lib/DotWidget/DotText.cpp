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

DotText::DotText(Adafruit_GFX *canvas, const uint8_t align, const String &text)
  : _canvas(canvas),
    _text(text),
    _align(align),
    _width(0),
    _height(0) {
  _canvas->getTextBounds(text, 0, 0, &_x, &_y, &_width, &_height);
}

void DotText::draw(const int16_t x, const int16_t y) const {
  int16_t begin = 0, end = 0;
  int16_t cursor_y = y - _y;

  while (_text[end]) {
    if (_text[end] == '\n') {
      _canvas->println();

      cursor_y = _canvas->getCursorY();
      begin++;
    } else if (_text[end + 1] == '\n' || !_text[end + 1]) {
      int16_t cursor_x = x - _x;
      String line = _text.substring(begin, end + 1);
      int16_t tx, ty;
      uint16_t tw, th;

      _canvas->getTextBounds(line, 0, 0, &tx, &ty, &tw, &th);
      cursor_x += alignOffset(_align & ALIGN_HORIZONTAL, _width, tw);

      _canvas->setCursor(cursor_x, cursor_y);
      _canvas->print(line);

      cursor_y = _canvas->getCursorY();
      begin = end + 1;
    }

    end++;
  }
}
