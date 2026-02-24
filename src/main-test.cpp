/**
 * test
 * Presents the display test.
 */

#include <AEGMIS_GV60.h>
#include <Arduino.h>

AEGMIS_GV60_SPI spi1(D1, D3, D5, D8);
AEGMIS_GV60_SPI spi2(D0, D2, D4, D7);
AEGMIS_GV60 display(&spi1, &spi2);

void setup() {
  display.begin();

  for (int i = 0; i < BC_WIDTH; i++) {
    for (int j = 0; j < BC_HEIGHT; j++) {
      bool bit = false;
      bit |= i >= 0 && i < 12 && j % 2 == 1;
      bit |= i >= 12 && i < 24 && i % 2 == 0;
      bit |= i >= 24 && i < 36 && (i + j) % 2;

      display.drawPixel(i, j, bit);
    }
  }

  display.display();
}

void loop() {}
