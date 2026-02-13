/**
 * test
 * Presents the display test.
 */

#include <Adafruit_BigClock.h>
#include <Arduino.h>

Adafruit_BigClock *canvas = new Adafruit_BigClock(
  new Adafruit_BigClockSPI(D2, D4, D6, D8),
  new Adafruit_BigClockSPI(D1, D3, D5, D7)
);

void setup() {
  canvas->begin();

  for (int i = 0; i < BC_WIDTH; i++) {
    for (int j = 0; j < BC_HEIGHT; j++) {
      bool bit = false;
      bit |= i >= 0 && i < 12 && j % 2 == 1;
      bit |= i >= 12 && i < 24 && i % 2 == 0;
      bit |= i >= 24 && i < 36 && (i + j) % 2;

      canvas->drawPixel(i, j, bit);
    }
  }

  canvas->display();
}

void loop() {}
