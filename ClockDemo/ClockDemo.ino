#include <Adafruit_GFX.h>
#include <BigClock.h>

GFXcanvas1 *canvas = NULL;
BigClock *bc = NULL;

void setup() {
  canvas = new GFXcanvas1(96,26);
  bc = new BigClock();
  bc->init();
}

void loop() {
  displayGFX();
}

void displayGFX()
{
  canvas->fillScreen(0);
  canvas->setCursor(1,1);
  canvas->print("Sheffield");
  canvas->setCursor(37,9);
  canvas->print("Hackspace");
  
  uint8_t *buffer = canvas->getBuffer();
  
  bc->output(buffer);
  bc->output(buffer); // Output twice otherwise it doesn't update the whole screen.
   
  delay(10000);
}
