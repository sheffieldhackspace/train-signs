/* mover
A pixel moves from left to right, and bounces.
It also moves upwards, or if the button is held, downwards.
Button to be wired to GND and PIN 3
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

int x = 5;
int x_dir = 1;
int y = 5;

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

    x += x_dir;
    if (x >= MAX_X || x <= MIN_X)
    {
        x_dir *= -1;
    }

    if (button == HIGH)
    {
        y += 1;
    }
    else
    {
        y -= 1;
    }

    if (y >= MAX_Y - 1)
    {
        y = MAX_Y - 1;
    }
    else if (y <= MIN_Y)
    {
        y = MIN_Y;
    }

    canvas->fillScreen(0);
    canvas->drawPixel(x, y, 1);
    uint8_t *buffer = canvas->getBuffer();

    bc->output(buffer);
}
