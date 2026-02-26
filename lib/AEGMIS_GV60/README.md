# AEGMIS_GV60

An Adafruit GFX compatible driver for the AEGMIS GV60 train station dot matrix display.

## Usage

```cpp
AEGMIS_GV60_SPI spi1(D1, D3, D5, D8);
AEGMIS_GV60_SPI spi2(D0, D2, D4, D7);
AEGMIS_GV60 display(&spi1, &spi2);

display.begin();
display.fillScreen(0);
display.setCursor(0, 0);
display.print("Hello");
display.display();
```

## Hardware

* Display dimensions: 96 × 26
* Operating voltage: 3.3V
* Power voltage: 5V

## Acknowledgements

Inspired by the BigClock library by Daniel Swann (Nottingham Hackspace), available at
https://github.com/daniel1111/BigClockSnake. The reverse engineering work of the display's
segment layout and SPI protocol was instrumental in the development of this library.
