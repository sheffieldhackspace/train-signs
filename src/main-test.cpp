/* checkerboard
Show a checkerboard pattern on the screen.
The corners are inverted
*/
#include <Arduino.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <BigClock.h>

#ifdef __AVR__
#define BOARDSEL_PIN 2
#define LATCH_PIN 4
#define WOUT_PIN 6
#endif
#ifdef ESP8266
#define BOARDSEL_PIN 12 // D6
#define LATCH_PIN 13    // D7
#define WOUT_PIN 14     // D5
#endif

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
    bool bit;
    for (int i = 0; i < MAX_X; i++)
    {
        for (int j = 0; j < MAX_Y; j++)
        {
            bit = false;
            bit |= i >= 0 && i < 12 && j % 2 == 1;
            bit |= i >= 12 && i < 24 && i % 2 == 0;
            bit |= i >= 24 && i < 36 && (i + j) % 2;

            canvas->drawPixel(i, j, bit);
        }
    }

    uint8_t *buffer = canvas->getBuffer();

    bc->output(buffer);
}
