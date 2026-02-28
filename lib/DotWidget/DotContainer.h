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

#pragma once

#include <Adafruit_GFX.h>
#include <vector>

#include "DotElement.h"

// Horizontal - bits 0-1
constexpr uint8_t ALIGN_LEFT = 0b00000001;
constexpr uint8_t ALIGN_RIGHT = 0b00000010;
constexpr uint8_t ALIGN_CENTER = 0b00000011;
constexpr uint8_t ALIGN_HORIZONTAL = 0b00000011;


// Vertical - bits 2-3
constexpr uint8_t ALIGN_TOP = 0b00000100;
constexpr uint8_t ALIGN_BOTTOM = 0b00001000;
constexpr uint8_t ALIGN_MIDDLE = 0b00001100;
constexpr uint8_t ALIGN_VERTICAL = 0b00001100;

inline int16_t alignOffset(uint8_t align, uint16_t containerSize, uint16_t elementSize) {
  if (containerSize <= elementSize) return 0;
  const auto distance = containerSize - elementSize;
  if ((align & ALIGN_HORIZONTAL) == ALIGN_CENTER || (align & ALIGN_VERTICAL) == ALIGN_MIDDLE) return distance / 2;
  if ((align & ALIGN_HORIZONTAL) == ALIGN_RIGHT || (align & ALIGN_VERTICAL) == ALIGN_BOTTOM) return distance;
  return 0;
}

class DotHorizontal : public DotElement {
public:
  explicit DotHorizontal(Adafruit_GFX *canvas, uint8_t align) : _canvas(canvas), _align(align), _width(0), _height(0) {}

  void add(DotElement *element);
  void draw(int16_t x, int16_t y) const override;
  [[nodiscard]] uint8_t align() const { return _align; }
  [[nodiscard]] uint16_t width() const override { return _width; }
  [[nodiscard]] uint16_t height() const override { return _height; }

private:
  Adafruit_GFX *_canvas;
  std::vector<DotElement *> _elements;
  uint8_t _align;
  uint16_t _width;
  uint16_t _height;
};

class DotVertical : public DotElement {
public:
  explicit DotVertical(Adafruit_GFX *canvas, uint8_t align) : _canvas(canvas), _align(align), _width(0), _height(0) {}

  void add(DotElement *element);
  void draw(int16_t x, int16_t y) const override;
  [[nodiscard]] uint8_t align() const { return _align; }
  [[nodiscard]] uint16_t width() const override { return _width; }
  [[nodiscard]] uint16_t height() const override { return _height; }

private:
  Adafruit_GFX *_canvas;
  std::vector<DotElement *> _elements;
  uint8_t _align;
  uint16_t _width;
  uint16_t _height;
};
