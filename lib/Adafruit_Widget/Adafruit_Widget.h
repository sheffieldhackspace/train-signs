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

#ifndef ADAFRUIT_WIDGET_H
#define ADAFRUIT_WIDGET_H

#include <Adafruit_GFX.h>
#include <Base64.h>
#include <Fonts/Org_01.h>

#define FRAMES_BEFORE 20
#define FRAMES_AFTER 20

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

class Adafruit_Widget {
public:
  Adafruit_Widget(Adafruit_GFX *canvas) : _canvas(canvas),
                                          _frame(0),
                                          _frames(FRAMES_BEFORE + FRAMES_AFTER),
                                          _image(nullptr),
                                          _image_width(0),
                                          _image_height(0),
                                          _text(""),
                                          _flash(false),
                                          _invert(false),
                                          _speed(5),
                                          _horizontal_align(LEFT),
                                          _text_wrap(false),
                                          _vertical_align(TOP) {}

  int16_t getAlign(int8_t a, uint16_t b, uint16_t d);
  int16_t getScroll(uint16_t b, uint16_t d);

  void advanceFrame();
  void begin();
  void print();
  void flash();
  void printText(int16_t x, int16_t y, uint16_t w, uint16_t h);

  void setFlash(bool flash) {
    _flash = flash;
  }

  void setImage(String image, uint16_t width, uint16_t height) {
    _image_width = width;
    _image_height = height;

    if (image == "") {
      delete(_image);
      _image = nullptr;
      return;
    }

    auto input = (char *) image.c_str();
    auto length = Base64.decodedLength(input, image.length()) + 1;

    _image = (char *) malloc(sizeof(char) * length);
    Base64.decode(_image, input, image.length());
  }

  void setInvert(bool invert) {
    _invert = invert;
    _canvas->invertDisplay(invert);
  }

  void setText(String text) {
    _frame = 0;
    _frames = FRAMES_BEFORE + FRAMES_AFTER;
    _text = text;
  }

  void setSpeed(uint8_t speed) {
    _speed = speed;
  }

  void setHorizontalAlign(HORIZONTAL_ALIGN align) {
    _horizontal_align = align;
  }

  void setTextWrap(bool wrap) {
    _text_wrap = wrap;
    _canvas->setTextWrap(wrap);
  }

  void setVerticalAlign(VERTICAL_ALIGN align) {
    _vertical_align = align;
  }

private:
  Adafruit_GFX *_canvas;
  uint16_t _frame;
  uint16_t _frames;

  char *_image;
  uint8_t _image_width;
  uint8_t _image_height;

  String _text;

  bool _flash;
  bool _invert;
  uint8_t _speed;
  bool _text_wrap;

  HORIZONTAL_ALIGN _horizontal_align;
  VERTICAL_ALIGN _vertical_align;
};


#endif //ADAFRUIT_WIDGET_H
