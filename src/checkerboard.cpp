/* checkerboard
Show a checkerboard pattern on the screen.
The corners are inverted
*/
#include <Arduino.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <BigClock.h>

#define MIN_X 0
#define MAX_X 96
#define MIN_Y 0
#define MAX_Y 26

GFXcanvas1 *canvas = NULL;
BigClock *bc = NULL;

void setup()
{
    canvas = new GFXcanvas1(MAX_X, MAX_Y);
    bc = new BigClock();
    bc->init();
}

void loop()
{
    // read the microswitch
    // int val = digitalRead(SWITCH_PIN);

    canvas->fillScreen(0);
    // checkerboard pattern (every x/y odd/even pixel is on/off)
    for (int i = 0; i < MAX_X; i++)
    {
        for (int j = 0; j < MAX_Y; j++)
        {
            if ((i + j) % 2 == 0)
            {
                canvas->drawPixel(i, j, 1);
            }
        }
    }

    // un-draw corners
    canvas->drawPixel(MIN_X, MIN_Y, 0);
    canvas->drawPixel(MAX_X - 1, MIN_Y, 1);
    canvas->drawPixel(MIN_X, MAX_Y - 1, 1);
    canvas->drawPixel(MAX_X - 1, MAX_Y - 1, 0);

    uint8_t *buffer = canvas->getBuffer();

    bc->output(buffer);
}
