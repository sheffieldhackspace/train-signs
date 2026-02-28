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

#include <sstream>
#include <string>
#include <Adafruit_GFX.h>

#include "DotContainer.h"
#include "DotElement.h"

class DotText : public DotVertical {
public:
  DotText(Adafruit_GFX *canvas, uint8_t align, const std::string &text);
  ~DotText() override;

private:
  class Line : public DotElement {
  public:
    Line(Adafruit_GFX *canvas, const std::string &text);

    void draw(int16_t x, int16_t y) const override;
    [[nodiscard]] uint16_t width() const override { return _width; }
    [[nodiscard]] uint16_t height() const override { return _height; }

  private:
    Adafruit_GFX *_canvas;
    std::string _text;
    int16_t _x;
    int16_t _y;
    uint16_t _width;
    uint16_t _height;
  };
};
