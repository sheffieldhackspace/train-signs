#!/usr/bin/env python

# Convert an image (.bmp, .gif) to a format accepted by the BigClock Widget API.
# Usage:
# $ ./image2bytes.py ./skull.gif

import base64
import sys
from PIL import Image

with Image.open(sys.argv[1]) as im:
    im = im.convert("1")
    print(base64.standard_b64encode(im.tobytes()).decode())
