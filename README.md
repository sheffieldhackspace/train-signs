# Train Station Clocks

The clocks in the space. See also Nottingham Hackspace's wiki:

<https://wiki.nottinghack.org.uk/wiki/BigClocks>

![Gif of "Sheffield Hackspace" moving around train clock sign](images/sign_moving.gif)

![Picture of train clock sign](images/train-sign.png)

## Screen size

96 × 26

## Building the project

```bash
pio run -e movingwords -t upload
```

## Hardware driver

The microcontroller is an Arduino Nano ESP32.

### Pinout

![Screenshot of a schematic presenting the pinout](images/schema.png)

See [`kicad project`](./kicad) for the latest version.

### Prototype

![Photo of the prototyped circuit board](images/proto.jpeg)

## Libraries

The BigClock library has been heavily rewritten by [`dredzik`](https://typedef.io/).
