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
  delete _image;
  delete _text;
}

// a - alignment value
// b - boundary dimension (i.e. width of the display)
// d - text dimension (i.e. width of the text)
// return - text coordinate (i.e. x position of the text)
int16_t DotWidget::calculateAlign(const int8_t a, const uint16_t b, const uint16_t d) {
  auto result = 0;

  if (const auto distance = b - d; distance > 0) {
    if (a == 0) {
      result = distance / 2;
    } else if (a == 1) {
      result = distance;
    }
  }

  return static_cast<int16_t>(result);
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
  _canvas->setTextColor(1);
  _canvas->setTextSize(1);
  _canvas->fillScreen(0);
}

void DotWidget::render() {
  int16_t x = 0, y = 0;

  _canvas->fillScreen(0);

  updateFlash();

  if (_text_wrap) {
    uint16_t widget_height = 0;

    if (_text != nullptr) {
      widget_height += _text->height();
    }

    if (_image != nullptr) {
      widget_height += _image->height() + 2;
    }

    y += calculateScroll(_canvas->height(), widget_height);

    if (_image != nullptr) {
      x += calculateAlign(_horizontal_align, _canvas->width(), _image->width());
      _image->draw(x, y);

      x = 0;
      y += _image->height() + 2;
    }

    if (_text != nullptr) {
      y += calculateAlign(_vertical_align, _canvas->height(), _text->height());
      _text->draw(x, y);
    }
  } else {
    uint16_t widget_width = 0;

    if (_text != nullptr) {
      widget_width += _text->width();
    }

    if (_image != nullptr) {
      widget_width += _image->width() * (_horizontal_align == CENTER ? 2 : 1);
    }

    x += calculateScroll(_canvas->width(), widget_width);
    x += calculateAlign(_horizontal_align, _canvas->width(), widget_width);

    if (_image != nullptr && _horizontal_align != RIGHT) {
      y = calculateAlign(_vertical_align, _canvas->height(), _image->height());
      _image->draw(x, y);
      x += _image->width();
    }

    if (_text != nullptr) {
      y = calculateAlign(_vertical_align, _canvas->height(), _text->height());
      _text->draw(x, y);
      x += _text->width();
    }

    if (_image != nullptr && _horizontal_align != LEFT) {
      y = calculateAlign(_vertical_align, _canvas->height(), _image->height());
      _image->draw(x, y);
    }
  }
}

void DotWidget::setFlashing(const bool flashing) {
  _flashing = flashing;
}

void DotWidget::setFont(const GFXfont *font) {
  _font = font;
  _canvas->setFont(font);
}

void DotWidget::setHorizontalAlign(const HORIZONTAL_ALIGN align) {
  _horizontal_align = align;
}

void DotWidget::setImage(const String &image, const uint16_t width, const uint16_t height) {
  delete _image;

  if (image.isEmpty()) {
    return;
  }

  _image = new DotImage(_canvas, image, width, height);
}

void DotWidget::setInverted(const bool inverted) {
  _inverted = inverted;
  _canvas->invertDisplay(inverted);
}

void DotWidget::setSpeed(const uint8_t speed) {
  _speed = speed;
}

void DotWidget::setText(const String &text) {
  delete _text;

  if (text.isEmpty()) {
    return;
  }

  _text = new DotText(_canvas, text, _horizontal_align);

  _frame = 0;
  _frames = FRAMES_BEFORE + FRAMES_AFTER;
}

void DotWidget::setTextSize(const uint8_t size) {
  _text_size = size;
  _canvas->setTextSize(size);
}

void DotWidget::setTextWrap(const bool wrap) {
  _text_wrap = wrap;
  _canvas->setTextWrap(wrap);
}

void DotWidget::setVerticalAlign(const VERTICAL_ALIGN align) {
  _vertical_align = align;
}
