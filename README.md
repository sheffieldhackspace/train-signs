# Train Station Clocks

The clocks in the space. See also Nottingham Hackspace's wiki:

<https://wiki.nottinghack.org.uk/wiki/BigClocks>

![Gif of "Sheffield Hackspace" moving around train clock sign](images/train-sign.gif)

![Picture of train clock sign](images/train-sign.png)

## Hardware

The microcontroller is a Seeed Studio XIAO ESP32C3. For hardware details, schematic, and PCB layout, see the [AEGMIS_GV60 repository](https://github.com/sheffieldhackspace/AEGMIS_GV60).

## Running the project

### Prerequisites

* [PlatformIO](https://platformio.org/) installed

### Installation

Flash the server environment to the device:

```bash
$ pio run -t clean -t upload -e aegmis
```

To run on an SSD1306 OLED display instead, use the `oled` environment:

```bash
$ pio run -t clean -t upload -e oled
```

For wiring the OLED, connect it to the XIAO ESP32C3 as follows:

| SSD1306 | XIAO ESP32C3 |
|---------|--------------|
| VCC     | 3V3          |
| GND     | GND          |
| SDA     | D4           |
| SCL     | D5           |

### Generation of compatible image data

```bash
$ ./icons/image2bytes.py ./icons/skull.gif
```

### Setting the status of the display

Once flashed, the display will show its IP address. You can then control it via HTTP:

```bash
$ curl http://ip_address:port/ --data '{
  "text": "Repair cafe @\nSheffield Hackspace",
  "text_size": 1,
  "text_wrap": true,
  "flashing": false,
  "inverted": false,
  "horizontal_align": "left",
  "vertical_align": "top",
  "image": "ABgAAZmAAf+AGf+YH//4DgAwDv/wfH/+fv+uP/UEP1VU8VF39xV/MVX8PV/8cf++f/8eD/+wDgAwH//4Gf+YAf+AAZmAABgA",
  "image_width": 24,
  "image_height": 24
}'
```

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `text` | string | `""` | The text to display. If too long, the display will scroll it. |
| `text_size` | integer | `1` | Text size multiplier, 1–5. |
| `text_wrap` | boolean | `true` | If true, overflow scrolls vertically; if false, horizontally. |
| `flashing` | boolean | `false` | If true, inverts colours every second producing a flashing animation. |
| `inverted` | boolean | `false` | Invert the colours of the display. |
| `speed` | integer | `5` | Scroll speed. |
| `horizontal_align` | string | `"left"` | `"left"`, `"center"`, `"right"`. |
| `vertical_align` | string | `"top"` | `"top"`, `"middle"`, `"bottom"`. |
| `image` | string | `""` | Base64-encoded image (output of the image2bytes script). |
| `image_width` | integer | `0` | Image width in pixels. |
| `image_height` | integer | `0` | Image height in pixels. |

### Integration tests

A test script is included that sends a series of curl requests and prompts you to verify each result visually:

```bash
$ chmod +x integration-test.sh && ./integration-test.sh 192.168.1.100
```

### Included libraries

* Adafruit GFX Library - The base for our implementation
* [AEGMIS_GV60](https://github.com/sheffieldhackspace/AEGMIS_GV60) - Provides an Adafruit GFX compatible driver for the train sign display; based on the reverse engineering work of [`daniel1111`](https://github.com/daniel1111/BigClockSnake) from the Nottingham Hackspace
* DotWidget - Provides a standard way of building a display widget for small IoT screens
