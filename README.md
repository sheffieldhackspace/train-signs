# Train Station Clocks

The clocks in the space. See also Nottingham Hackspace's wiki:

<https://wiki.nottinghack.org.uk/wiki/BigClocks>

![Gif of "Sheffield Hackspace" moving around train clock sign](images/sign_moving.gif)

![Picture of train clock sign](images/train-sign.png)

## Clock information

* Dimensions: 96 × 26
* Operating voltage: 3.3V
* Power voltage: 5V

## Hardware driver

The microcontroller is a XIAO Studio Seeed ESP32C3.

### Pinout

![Screenshot of a schematic presenting the pinout](images/schema.png)

See [`kicad project`](./kicad) for the latest version.

### Prototype

![Photo of the prototyped circuit board](images/proto.jpeg)

## Running the project

### Installation

```bash
$ pio run -t clean -t upload -e server
```

### Generation of compatible image data

```bash
$ ./icons/image2bytes.py ./icons/skull.gif
```

### Setting the status of the display

```bash
$ curl http://ip_address:port/ --data '{
  "text": "Repair cafe @\nSheffield Hackspace",
  "text_wrap": false,
  "flash": false,
  "invert": false,
  "horizontal_align": -1,
  "vertical_align": 0,
  "image": "ABgAAZmAAf+AGf+YH//4DgAwDv/wfH/+fv+uP/UEP1VU8VF39xV/MVX8PV/8cf++f/8eD/+wDgAwH//4Gf+YAf+AAZmAABgA",
  "image_width": 24,
  "image_height": 24
}'
```

* text - The text to be displayed. If it is too long, the display will scroll the text accordingly.
* text_wrap - Should the text be wrapped or not. If true, the overflow will be scrolled vertially; if false, horizontally.
* flash - Set true to invert the colours every second producing a flashing animation
* invert - Invert the colours of the display
* horizontal_align - -1 left, 0 centre, 1 right
* vertical_align - -1 top, 0 centre, 1 bottom
* image - base64 of an image (output of the other script)
* image_width - image width
* image_height - image height

### Included libraries

* Adafruit GFX Library - The base for our implementation
* Adafruit BigClock - Provides an Adafruit GFX compatible driver for the display; Based on the BigClock library made by the Nottingham Hackspace, heavily rewritten by [`dredzik`](https://typedef.io/)
* Adafruit Widget - Provided a standard way of building a display widget for small IoT screens
