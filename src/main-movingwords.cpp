/**
 * movingwords
 * Scroll two lines of text around on the display.
*/

#include <AEGMIS_GV60.h>
#include <Arduino.h>

#define MAX_X_DEVIATION 36
#define MAX_Y_DEVIATION 9

uint8_t scroll = 0;
int reverse = 0;

uint8_t scrolly = 0;
int reversey = 0;

AEGMIS_GV60_SPI spi1(D1, D3, D5, D8);
AEGMIS_GV60_SPI spi2(D0, D2, D4, D7);
AEGMIS_GV60 display(&spi1, &spi2);

void setup() {
  display.begin();
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

  display.fillScreen(0);
  display.setCursor(1 + scroll, 1 + scrolly);
  display.print("Sheffield");
  display.setCursor(37 - scroll, 9 + scrolly);
  display.print("Hackspace");

  display.display();

  delay(50);
}
