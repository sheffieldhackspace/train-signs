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

#include <Base64.h>

#include "DotImage.h"

DotImage::DotImage(Adafruit_GFX *canvas, const String &image, uint16_t width, uint16_t height)
  : _canvas(canvas),
    _bitmap(nullptr),
    _width(width),
    _height(height) {
  const auto input = const_cast<char *>(image.c_str());
  const auto input_length = static_cast<int>(image.length());

  const auto bitmap_length = Base64.decodedLength(input, input_length) + 1;
  _bitmap = new uint8_t[bitmap_length];

  Base64.decode(reinterpret_cast<char *>(_bitmap), input, input_length);
}

DotImage::~DotImage() {
  delete[] _bitmap;
}

void DotImage::draw(int16_t x, int16_t y) const {
  const int16_t srcY = max(static_cast<int16_t>(0), static_cast<int16_t>(-y));
  const int16_t dstY = max(static_cast<int16_t>(0), static_cast<int16_t>(y));
  const uint16_t visibleHeight = min(static_cast<int16_t>(_height) - srcY, _canvas->height() - dstY);

  if (visibleHeight <= 0) return;

  const auto rowBytes = (_width + 7) / 8;
  const auto *visibleBuffer = _bitmap + (srcY * rowBytes);

  _canvas->drawBitmap(x, dstY, visibleBuffer, _width, visibleHeight, 1, 0);
}
