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

DotText::Line::Line(Adafruit_GFX *canvas, const std::string &text)
  : _canvas(canvas), _text(text), _width(0), _height(0) {
  _canvas->getTextBounds(_text.c_str(), 0, 0, &_x, &_y, &_width, &_height);

  int16_t ref_x, ref_y;
  uint16_t ref_w, ref_h;
  _canvas->getTextBounds("Ag", 0, 0, &ref_x, &ref_y, &ref_w, &ref_h);

  if (_height % ref_h != 0) {
    _height += ref_h - (_height % ref_h);
  }
}

void DotText::Line::draw(const int16_t x, const int16_t y) const {
  _canvas->setCursor(x - _x, y - _y);
  _canvas->print(_text.c_str());
}

DotText::DotText(Adafruit_GFX *canvas, const uint8_t align, const std::string &text)
  : DotVertical(canvas, align) {
  std::istringstream stream(text);
  std::string lt;

  while (std::getline(stream, lt)) {
    add(new Line(canvas, lt));
  }
}

DotText::~DotText() {
  for (const auto *element: _elements) {
    delete element;
  }
}
