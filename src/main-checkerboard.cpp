/**
 * checkerboard
 * Show a checkerboard pattern on the screen.
 */
#include <Adafruit_BigClock.h>
#include <Arduino.h>

Adafruit_BigClock *canvas = new Adafruit_BigClock(
  new Adafruit_BigClockSPI(D2, D4, D6, D8),
  new Adafruit_BigClockSPI(D1, D3, D5, D7)
);

void setup() {
    canvas->begin();

    for (int i = 0; i < BIG_CLOCK_WIDTH; i++) {
        for (int j = 0; j < BIG_CLOCK_HEIGHT; j++) {
            canvas->drawPixel(i, j, (i + j) % 2);
        }
    }

    canvas->display();
}

void loop() {
}
