# convert an image (.bmp) to bytes, where
#  each byte represents 8 bits of the image
#  columns are wrapped
# example
#   py python/image2bytes.py python/example_image_1.bmp | curl -X POST http://10.3.50.200:80 --data-binary '@-'

import sys
from PIL import Image
from io import BytesIO


def getbytes(bits):
    done = False
    while not done:
        byte = 0
        for _ in range(0, 8):
            try:
                bit = next(bits)
            except StopIteration:
                bit = 0
                done = True
            byte = (byte << 1) | bit
        yield byte


bits = []

with Image.open(sys.argv[1]) as im:
    stream = BytesIO()
    im.save(stream, "BMP")
    im = im.convert("1")
    data = im.getdata()
    for i in data:
        # print(1 if i == 255 else 0, end="")
        bits.append(1 if i == 255 else 0)

# with open(sys.argv[1], "rb") as image:
#     f = image.read()
#     b = bytearray(f)

# sys.stdout.buffer.write(stream.getbuffer())
sys.stdout.buffer.write(bytes([b for b in getbytes(iter(bits))]))

# print(stream)
