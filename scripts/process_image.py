#!/usr/bin/env python3

"""Processes an image:
1) annotates the image with the current date and time
2) downscales and crops the image"""

import argparse
import sys
from datetime import datetime
from os.path import exists

from PIL import Image, ImageDraw, ImageFont


def annotate(filename, font_path):
    """Annotates the image with the current date and time using the given font."""

    date_str = datetime.now().strftime('%d.%m.%Y %H:%M')

    base = Image.open(filename)

    if not exists(font_path):
        print("Font file '{} does not exist, stopping.".format(font_path),
              file=sys.stderr)
        return

    font = ImageFont.truetype(font_path, 30)
    draw = ImageDraw.Draw(base)

    text_size = draw.textsize(date_str, font)
    # pylint: disable=invalid-name
    x = (base.width / 2) - (text_size[0] / 2)
    y = 10

    draw.text((x, y), date_str, font=font, fill=(255, 255, 255))

    base.save(filename)


def crop(image, filename):
    """Crops the input image and overwrites the original image with the cropped
    image."""
    dimensions = (0, 0, image.width, int(image.height * 0.7))
    region = image.crop(dimensions)
    region.save(filename)


def downscale(filename):
    """Downscales the image to half of its original size and returns the
    downscaled image."""
    img = Image.open(filename)
    small = img.resize((int(img.width / 2), int(img.height / 2)),
                       Image.BILINEAR)

    return small


def main():
    """Module main function."""
    parser = argparse.ArgumentParser(description='Processes an image.')
    parser.add_argument('operation', type=str, help='operation to perform: '
                        'annotate (add current date and time) or '
                        'downscale (downscale and crop)')
    parser.add_argument('image_name', type=str, help='image to manipulate')
    parser.add_argument('--font', type=str, help='font file to use in annotation')

    args = parser.parse_args()

    if args.operation not in ('annotate', 'downscale'):
        print("Unknown operation '{}.".format(args.operation),
              file=sys.stderr)
        exit(1)

    if args.operation == 'annotate' and not args.font:
        print('The font must be provided with --font when annotating', file=sys.stderr)
        exit(1)

    if not exists(args.image_name):
        print("Image file '{}' does not exist.".format(args.image_name),
              file=sys.stderr)
        exit(1)

    if args.operation == 'annotate':
        annotate(args.image_name, args.font)
    elif args.operation == 'downscale':
        crop(downscale(args.image_name), args.image_name)

    return 0


if __name__ == '__main__':
    main()
