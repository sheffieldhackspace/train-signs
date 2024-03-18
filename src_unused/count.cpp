/* count
Display a number on the display.
Count up every time a button is pressed.
Requires a button to be wired to GND and PIN 3
*/
#include <Arduino.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <BigClock.h>

#define MIN_X 0
#define MAX_X 96
#define MIN_Y 0
#define MAX_Y 26

#define SWITCH_PIN 3

GFXcanvas1 *canvas = NULL;
BigClock *bc = NULL;

uint64_t count = 0;

int last_button = LOW;

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
    int button = digitalRead(SWITCH_PIN);

    if (button == HIGH && last_button == LOW)
    {
        count++;
        last_button = button;
    }
    else
    {
        last_button = button;
        return;
    }

    canvas->fillScreen(0);
    canvas->setCursor(1, 1);
    // convert count to binary string
    char buff[64];
    itoa(count, buff, 2);
    canvas->print(buff);

    uint8_t *buffer = canvas->getBuffer();
    bc->output(buffer);
    bc->output(buffer); // Output twice otherwise it doesn't update the whole screen.
}
