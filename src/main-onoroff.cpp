/* onoroff
Displays "ON" or "OFF" based on button state
Button must be wired to GND and PIN 3
*/
#include <Arduino.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <BigClock.h>

#define MAX_X 96
#define MAX_Y 26

#define SWITCH_PIN 3

GFXcanvas1 *canvas = NULL;
BigClock *bc = NULL;

void setup()
{
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    canvas = new GFXcanvas1(MAX_X, MAX_Y);
    bc = new BigClock();
    bc->init();
}

void loop()
{
    // read the microswitch
    int val = digitalRead(SWITCH_PIN);

    canvas->fillScreen(0);
    canvas->setCursor(1, 1);
    if (val == HIGH)
        canvas->print("ON");
    else
        canvas->print("OFF");

    uint8_t *buffer = canvas->getBuffer();

    bc->output(buffer);
    bc->output(buffer); // Output twice otherwise it doesn't update the whole screen.
}
