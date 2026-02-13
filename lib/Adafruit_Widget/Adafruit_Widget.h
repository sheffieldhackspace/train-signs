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

#include <Adafruit_BigClock.h>
#include <Base64.h>
#include <Fonts/Org_01.h>

#define FRAMES_BEFORE 20
#define FRAMES_AFTER 20

enum TEXT_ALIGN {
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
  Adafruit_Widget(Adafruit_BigClock *canvas)
    : _canvas(canvas), _frame(0), _frames(FRAMES_BEFORE + FRAMES_AFTER),
      _image(nullptr), _image_width(0), _image_height(0), _message(nullptr),
      _flash(false), _invert(false), _speed(5), _text_align(LEFT), _text_wrap(false), _vertical_align(TOP) {}

  void applyAlign(int8_t a, int16_t b, int16_t d, int16_t *c);
  void applyFlash();
  void applyScroll(int16_t b, int16_t d, int16_t *c);
  void begin();
  void display();
  void printText(int16_t x, int16_t y, uint16_t w, uint16_t h);
  void waitForNextFrame();

  void setFlash(bool flash) {
    _flash = flash;
  }

  void setImage(String *image) {
    if (*image == "") {
      _image = nullptr;
      _image_width = 0;
      _image_height = 0;
      return;
    }

    _image = (char *) malloc(sizeof(char) * 73);
    Base64.decode(_image, (char *) image->c_str(), image->length());
    _image_width = 24;
    _image_height = 24;
  }

  void setInvert(bool invert) {
    _invert = invert;
    _canvas->invertDisplay(invert);
  }

  void setMessage(String *message) {
    _frame = 0;
    _frames = FRAMES_BEFORE + FRAMES_AFTER;
    _message = message;
  }

  void setSpeed(uint8_t speed) {
    _speed = speed;
  }

  void setTextAlign(TEXT_ALIGN align) {
    _text_align = align;
  }

  void setTextWrap(bool wrap) {
    _text_wrap = wrap;
    _canvas->setTextWrap(wrap);
  }

  void setVerticalAlign(VERTICAL_ALIGN align) {
    _vertical_align = align;
  }

private:
  Adafruit_BigClock *_canvas;
  uint16_t _frame;
  uint16_t _frames;

  char *_image;
  uint8_t _image_width;
  uint8_t _image_height;

  String *_message;

  bool _flash;
  bool _invert;
  uint8_t _speed;
  TEXT_ALIGN _text_align;
  bool _text_wrap;
  VERTICAL_ALIGN _vertical_align;
};


#endif //ADAFRUIT_WIDGET_H
