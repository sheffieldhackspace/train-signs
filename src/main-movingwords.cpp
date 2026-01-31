/* movingwords
Scroll two lines of text around on the display
*/

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <BigClock.h>

#define MAX_X_DEVIATION 36
#define MAX_Y_DEVIATION 9

GFXcanvas1 *canvas = NULL;
BigClock *bc = NULL;

uint8_t scroll = 0;
int reverse = 0;

uint8_t scrolly = 0;
int reversey = 0;

void setup() {
  canvas = new GFXcanvas1(96, 26);
  bc = new BigClock(canvas->getBuffer());
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

  bc->output();

  delay(50);
}
