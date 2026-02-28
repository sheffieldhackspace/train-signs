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

#include <string>
#include <Adafruit_GFX.h>

#include "DotContainer.h"
#include "DotImage.h"
#include "DotText.h"

constexpr int16_t FRAMES_BEFORE = 20;
constexpr int16_t FRAMES_AFTER = 20;

class DotWidget {
public:
  explicit DotWidget(Adafruit_GFX *canvas) : _canvas(canvas),
                                             _frame(0),
                                             _frames(FRAMES_BEFORE + FRAMES_AFTER),
                                             _align(ALIGN_CENTER | ALIGN_MIDDLE),
                                             _container(nullptr),
                                             _image(nullptr),
                                             _text(nullptr),
                                             _font(nullptr),
                                             _text_size(1),
                                             _text_wrap(false),
                                             _flashing(false),
                                             _inverted(false),
                                             _speed(5) {}

  ~DotWidget();
  void advanceFrame();
  // Must be called before render() or any set* methods
  void begin() const;
  void rebuild();
  void render();

  void setAlign(uint8_t align);
  void setFlashing(bool flashing);
  void setImage(const String &image, uint16_t width, uint16_t height);
  void setInverted(bool inverted);
  void setSpeed(uint8_t speed);
  void setText(const String &text, const GFXfont *font = nullptr, uint8_t size = 1, bool wrap = false);

private:
  int16_t calculateScroll(uint16_t b, uint16_t d);
  void updateFlash();

  Adafruit_GFX *_canvas;
  uint16_t _frame;
  uint16_t _frames;

  uint8_t _align;
  DotElement *_container;
  DotImage *_image;
  DotText *_text;

  const GFXfont *_font;
  uint8_t _text_size;
  bool _text_wrap;

  bool _flashing;
  bool _inverted;
  uint8_t _speed;
};
