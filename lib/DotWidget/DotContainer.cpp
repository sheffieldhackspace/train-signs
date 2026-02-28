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

#include "DotContainer.h"

void DotHorizontal::add(DotElement *element) {
  _elements.push_back(element);
  _width += element->width();
  _height = max(_height, element->height());
}

void DotHorizontal::draw(int16_t x, int16_t y) const {
  int cursor_x = x;

  for (const auto *element: _elements) {
    int16_t cursor_y = y + alignOffset(_align & ALIGN_VERTICAL, _height, element->height());

    element->draw(cursor_x, cursor_y);
    cursor_x += element->width();
  }
}

void DotVertical::add(DotElement *element) {
  _elements.push_back(element);
  _width = max(_width, element->width());
  _height += element->height();
}

void DotVertical::draw(const int16_t x, const int16_t y) const {
  int16_t cursor_y = y;

  for (const auto *element: _elements) {
    int16_t cursor_x = x + alignOffset(_align & ALIGN_HORIZONTAL, _width, element->width());

    element->draw(cursor_x, cursor_y);
    cursor_y += element->height();
  }
}
