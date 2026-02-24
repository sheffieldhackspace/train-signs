/**
 * checkerboard
 * Show a checkerboard pattern on the screen.
 */
#include <AEGMIS_GV60.h>
#include <Arduino.h>

AEGMIS_GV60 *display = new AEGMIS_GV60(
  new AEGMIS_GV60_SPI(D2, D4, D6, D8),
  new AEGMIS_GV60_SPI(D1, D3, D5, D7)
);

void setup() {
  display->begin();

  for (int i = 0; i < BC_WIDTH; i++) {
    for (int j = 0; j < BC_HEIGHT; j++) {
      display->drawPixel(i, j, (i + j) % 2);
    }
  }

  display->display();
}

void loop() {}
