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

#include "DotWidget.h"

DotWidget::~DotWidget() {
  delete _container;
  delete _image;
  delete _text;
}

// b - boundary dimension (i.e. width of the display)
// d - text dimension (i.e. width of the text)
// return - text coordinate (i.e. x position of the text)
int16_t DotWidget::calculateScroll(const uint16_t b, const uint16_t d) {
  auto result = 0;

  if (const auto distance = d - b; distance > 0) {
    _frames = FRAMES_BEFORE + distance + FRAMES_AFTER;

    if (_frame < FRAMES_BEFORE) {
      result = 0;
    } else if (_frame < FRAMES_BEFORE + distance) {
      result = -(_frame - FRAMES_BEFORE);
    } else {
      result = -distance;
    }
  }

  return static_cast<int16_t>(result);
}

void DotWidget::updateFlash() {
  if (_flashing && _frame % (_speed / 2) == 0) {
    setInverted(!_inverted);
  }
}

void DotWidget::advanceFrame() {
  _frame++;

  if (_frame >= _frames) {
    _frame = 0;
  }

  delay(1000 / _speed);
}

void DotWidget::begin() const {
  _canvas->setFont(_font);
  _canvas->setTextSize(_text_size);
  _canvas->setTextWrap(_text_wrap);

  _canvas->setTextColor(1);
  _canvas->fillScreen(0);
}

void DotWidget::rebuild() {
  delete _container;
  _container = nullptr;

  if (_text_wrap) {
    auto *container = new DotVertical(_canvas, _align);
    if (_image) container->add(_image);
    if (_text) container->add(_text);
    _container = container;
  } else if (_image && !_text) {
    auto *container = new DotHorizontal(_canvas, _align);
    container->add(_image);
    _container = container;
  } else {
    auto *container = new DotHorizontal(_canvas, _align);
    if (_image && (_align & ALIGN_LEFT)) container->add(_image);
    if (_text) container->add(_text);
    if (_image && (_align & ALIGN_RIGHT)) container->add(_image);
    _container = container;
  }
}

void DotWidget::render() {
  if (_container == nullptr) rebuild();
  if (_container == nullptr) return;

  int16_t cursor_x = 0, cursor_y = 0;

  _canvas->fillScreen(0);
  updateFlash();

  if (_text_wrap) {
    cursor_y += calculateScroll(_canvas->height(), _container->height());
  } else {
    cursor_x += calculateScroll(_canvas->width(), _container->width());
  }

  cursor_x += alignOffset(_align & ALIGN_HORIZONTAL, _canvas->width(), _container->width());
  cursor_y += alignOffset(_align & ALIGN_VERTICAL, _canvas->height(), _container->height());

  _container->draw(cursor_x, cursor_y);
}

void DotWidget::setAlign(const uint8_t align) {
  _align = align;

  delete _container;
  _container = nullptr;
}

void DotWidget::setFlashing(const bool flashing) {
  _flashing = flashing;
}

void DotWidget::setImage(const String &image, const uint16_t width, const uint16_t height) {
  delete _image;
  _image = nullptr;

  if (!image.isEmpty()) {
    _image = new DotImage(_canvas, image, width, height);
  }

  delete _container;
  _container = nullptr;
}

void DotWidget::setInverted(const bool inverted) {
  _inverted = inverted;
  _canvas->invertDisplay(inverted);
}

void DotWidget::setSpeed(const uint8_t speed) {
  _speed = speed;
}

void DotWidget::setText(const String &text, const GFXfont *font, const uint8_t size, const bool wrap) {
  delete _text;
  _text = nullptr;

  _font = font;
  _text_size = size;
  _text_wrap = wrap;

  _canvas->setFont(_font);
  _canvas->setTextSize(_text_size);
  _canvas->setTextWrap(_text_wrap);

  if (!text.isEmpty()) {
    _text = new DotText(_canvas, _align, text.c_str());
  }

  _frame = 0;
  _frames = FRAMES_BEFORE + FRAMES_AFTER;

  delete _container;
  _container = nullptr;
}
