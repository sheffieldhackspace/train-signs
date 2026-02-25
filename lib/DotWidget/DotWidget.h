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
#include <Base64.h>

constexpr int16_t FRAMES_BEFORE = 20;
constexpr int16_t FRAMES_AFTER = 20;

enum HORIZONTAL_ALIGN {
  LEFT = -1,
  CENTER = 0,
  RIGHT = 1,
};

enum VERTICAL_ALIGN {
  TOP = -1,
  MIDDLE = 0,
  BOTTOM = 1,
};

class DotWidget {
public:
  explicit DotWidget(Adafruit_GFX *canvas) : _canvas(canvas),
                                             _dirty(true),
                                             _frame(0),
                                             _frames(FRAMES_BEFORE + FRAMES_AFTER),
                                             _image_bitmap(nullptr),
                                             _image_width(0),
                                             _image_height(0),
                                             _text(""),
                                             _text_bitmap(nullptr),
                                             _text_width(0),
                                             _text_height(0),
                                             _flashing(false),
                                             _font(nullptr),
                                             _inverted(false),
                                             _speed(5),
                                             _text_wrap(false),
                                             _horizontal_align(LEFT),
                                             _vertical_align(TOP) {}

  ~DotWidget();
  void advanceFrame();
  // Must be called before render() or any set* methods
  void begin() const;
  void render();

  void setFlashing(bool flashing);
  void setFont(const GFXfont *font);
  void setHorizontalAlign(HORIZONTAL_ALIGN align);
  void setImage(const String &image, uint16_t width, uint16_t height);
  void setInverted(bool inverted);
  void setSpeed(uint8_t speed);
  void setText(const String &text);
  void setTextWrap(bool wrap);
  void setVerticalAlign(VERTICAL_ALIGN align);

private:
  static int16_t calculateAlign(int8_t a, uint16_t b, uint16_t d);
  int16_t calculateScroll(uint16_t b, uint16_t d);
  void renderText();
  void updateFlash();

  Adafruit_GFX *_canvas;
  bool _dirty;
  uint16_t _frame;
  uint16_t _frames;

  uint8_t *_image_bitmap;
  uint16_t _image_width;
  uint16_t _image_height;

  String _text;
  uint8_t *_text_bitmap;
  uint16_t _text_width;
  uint16_t _text_height;

  bool _flashing;
  const GFXfont *_font;
  bool _inverted;
  uint8_t _speed;
  bool _text_wrap;

  HORIZONTAL_ALIGN _horizontal_align;
  VERTICAL_ALIGN _vertical_align;
};
