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
  delete[] _image_bitmap;
  delete[] _text_bitmap;
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

void DotWidget::renderText() {
  delete[] _text_bitmap;
  _text_bitmap = nullptr;

  if (_text.isEmpty()) {
    return;
  }

  int16_t text_x, text_y;
  _canvas->getTextBounds(_text, 0, 0, &text_x, &text_y, &_text_width, &_text_height);
  GFXcanvas1 offscreen(_text_width, _text_height);
  int16_t begin = 0, end = 0;
  text_y = -text_y;

  offscreen.fillScreen(0);
  offscreen.setTextColor(1);
  offscreen.setTextSize(1);
  offscreen.setTextWrap(_text_wrap);
  offscreen.setFont(_font);

  while (_text[end]) {
    if (_text[end] == '\n') {
      offscreen.println();

      text_y = offscreen.getCursorY();
      begin++;
    } else if (_text[end + 1] == '\n' || !_text[end + 1]) {
      String line = _text.substring(begin, end + 1);
      int16_t tx, ty;
      uint16_t tw, th;

      offscreen.getTextBounds(line, 0, 0, &tx, &ty, &tw, &th);
      offscreen.setCursor(calculateAlign(_horizontal_align, _text_width, tw), text_y);

      offscreen.print(line);

      text_y = offscreen.getCursorY();
      begin = end + 1;
    }

    end++;
  }

  const auto size = ((offscreen.width() + 7) / 8) * offscreen.height();
  _text_bitmap = new uint8_t[size];
  memcpy(_text_bitmap, offscreen.getBuffer(), size);
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
  _canvas->setTextSize(1);
  _canvas->fillScreen(0);
  _canvas->setTextColor(1);
}

void DotWidget::render() {
  if (_dirty) {
    renderText();
    _dirty = false;
  }

  int16_t x = 0, y = 0;

  _canvas->fillScreen(0);

  updateFlash();

  if (_text_wrap) {
    uint16_t widget_height = _text_height;

    if (_image_height) {
      widget_height += _image_height + 2;
    }

    y += calculateScroll(_canvas->height(), widget_height);

    if (_image_bitmap != nullptr) {
      x += calculateAlign(_horizontal_align, _canvas->width(), _image_width);

      _canvas->drawBitmap(x, y, _image_bitmap, _image_width, _image_height, 1, 0);

      x = 0;
      y += _image_height + 2;
    }

    y += calculateAlign(_vertical_align, _canvas->height(), _text_height);
    _canvas->drawBitmap(x, y, _text_bitmap, _text_width, _text_height, 1, 0);
  } else {
    uint16_t widget_width = _text_width + _image_width;

    if (_horizontal_align == CENTER) {
      widget_width += _image_width;
    }

    x += calculateScroll(_canvas->width(), widget_width);
    x += calculateAlign(_horizontal_align, _canvas->width(), widget_width);

    if (_image_bitmap != nullptr && _horizontal_align != RIGHT) {
      y = calculateAlign(_vertical_align, _canvas->height(), _image_height);

      _canvas->drawBitmap(x, y, _image_bitmap, _image_width, _image_height, 1, 0);

      x += _image_width;
      y = 0;
    }

    y += calculateAlign(_vertical_align, _canvas->height(), _text_height);
    _canvas->drawBitmap(x, y, _text_bitmap, _text_width, _text_height, 1, 0);
    x += _text_width;

    if (_image_bitmap != nullptr && _horizontal_align != LEFT) {
      y = calculateAlign(_vertical_align, _canvas->height(), _image_height);

      _canvas->drawBitmap(x, y, _image_bitmap, _image_width, _image_height, 1, 0);
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
  _dirty = true;
  _horizontal_align = align;
}

void DotWidget::setImage(const String &image, const uint16_t width, const uint16_t height) {
  _image_width = width;
  _image_height = height;

  delete[] _image_bitmap;
  _image_bitmap = nullptr;

  if (image.isEmpty()) {
    return;
  }

  const auto input = const_cast<char *>(image.c_str());
  const auto input_length = static_cast<int>(image.length());

  const auto bitmap_length = Base64.decodedLength(input, input_length) + 1;
  _image_bitmap = new uint8_t[bitmap_length];

  Base64.decode(reinterpret_cast<char *>(_image_bitmap), input, input_length);
}

void DotWidget::setInverted(const bool inverted) {
  _inverted = inverted;
  _canvas->invertDisplay(inverted);
}

void DotWidget::setSpeed(const uint8_t speed) {
  _speed = speed;
}

void DotWidget::setText(const String &text) {
  _dirty = true;
  _frame = 0;
  _frames = FRAMES_BEFORE + FRAMES_AFTER;
  _text = text;
}

void DotWidget::setTextWrap(const bool wrap) {
  _dirty = true;
  _text_wrap = wrap;
  _canvas->setTextWrap(wrap);
}

void DotWidget::setVerticalAlign(const VERTICAL_ALIGN align) {
  _vertical_align = align;
}
