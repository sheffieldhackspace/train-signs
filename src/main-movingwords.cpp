/**
 * movingwords
 * Scroll two lines of text around on the display.
*/

#include <Adafruit_BigClock.h>
#include <Arduino.h>

#define MAX_X_DEVIATION 36
#define MAX_Y_DEVIATION 9

uint8_t scroll = 0;
int reverse = 0;

uint8_t scrolly = 0;
int reversey = 0;

Adafruit_BigClock *canvas = new Adafruit_BigClock(
  new Adafruit_BigClockSPI(D2, D4, D6, D8),
  new Adafruit_BigClockSPI(D1, D3, D5, D7)
);

void setup() {
  canvas->begin();
}

void loop() {
  if (scroll > MAX_X_DEVIATION)
    reverse = 1;
  else if (scroll < 1)
    reverse = 0;

  if (scrolly > MAX_Y_DEVIATION)
    reversey = 1;
  else if (scrolly < 1)
    reversey = 0;

  if (reverse)
    scroll--;
  else
    scroll++;

  if (reversey)
    scrolly--;
  else
    scrolly++;

  canvas->fillScreen(0);
  canvas->setCursor(1 + scroll, 1 + scrolly);
  canvas->print("Sheffield");
  canvas->setCursor(37 - scroll, 9 + scrolly);
  canvas->print("Hackspace");

  canvas->display();

  delay(50);
}
