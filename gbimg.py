#! /usr/bin/env python3
##
##  gbimg - turn PC image files into Game Boy tilesets
##  Copyright (C) 2016 Delwink, LLC
##
##  Redistributions, modified or unmodified, in whole or in part, must retain
##  applicable notices of copyright or other legal privilege, these conditions,
##  and the following license terms and disclaimer.  Subject to these
##  conditions, each holder of copyright or other legal privileges, author or
##  assembler, and contributor of this work, henceforth "licensor", hereby
##  grants to any person who obtains a copy of this work in any form:
##
##  1. Permission to reproduce, modify, distribute, publish, sell, sublicense,
##  use, and/or otherwise deal in the licensed material without restriction.
##
##  2. A perpetual, worldwide, non-exclusive, royalty-free, gratis, irrevocable
##  patent license to make, have made, provide, transfer, import, use, and/or
##  otherwise deal in the licensed material without restriction, for any and
##  all patents held by such licensor and necessarily infringed by the form of
##  the work upon distribution of that licensor's contribution to the work
##  under the terms of this license.
##
##  NO WARRANTY OF ANY KIND IS IMPLIED BY, OR SHOULD BE INFERRED FROM, THIS
##  LICENSE OR THE ACT OF DISTRIBUTION UNDER THE TERMS OF THIS LICENSE,
##  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
##  A PARTICULAR PURPOSE, AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS,
##  ASSEMBLERS, OR HOLDERS OF COPYRIGHT OR OTHER LEGAL PRIVILEGE BE LIABLE FOR
##  ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN ACTION OF CONTRACT,
##  TORT, OR OTHERWISE ARISING FROM, OUT OF, OR IN CONNECTION WITH THE WORK OR
##  THE USE OF OR OTHER DEALINGS IN THE WORK.
##

from wand.image import Image

_PALETTE = {
    0:   3,
    85:  2,
    170: 1,
    255: 0
}

def map_palette(color):
    if color not in _PALETTE:
        raise ValueError('{} is not a valid color level'.format(color))

    return _PALETTE[color]

class Tileset:
    def __init__(self, img, data, bank=None):
        self._img = img
        self._data = None
        self._data_var = data

        if type(bank) is str:
            self._bank_var = bank
            self._bank_num = None
        elif type(bank) is int:
            self._bank_var = None
            self._bank_num = bank
        elif bank is None:
            self._bank_var = None
            self._bank_num = None
        else:
            raise TypeError('bank must be str, int, or None')

    def set_bank_num(self, num):
        self._bank_num = num

    def _convert(self):
        with Image(filename=self._img) as img:
            width, height = img.size
            if height != 8:
                raise ValueError('{} height is {}'.format(self, height))

            num_tiles = width / 8
            if num_tiles % 1 != 0:
                raise ValueError('Irregular tile width in {}'.format(self))

            num_tiles = int(num_tiles)
            img.depth = 8
            blob = img.make_blob(format='RGB')

        encoded = []
        for tile in range(num_tiles):
            encoded_tile = []

            for i in range(0, width * 8 * 3, width * 3):
                byte1 = 0
                byte2 = 0
                bit = 0x80

                for j in range(0, 8 * 3, 3):
                    color = map_palette(blob[(i + j) + ((8 * 3) * tile)])

                    if color & 0x01:
                        byte1 |= bit
                    if color & 0x02:
                        byte2 |= bit

                    bit >>= 1

                encoded_tile.append(byte1)
                encoded_tile.append(byte2)

            encoded += encoded_tile

        return bytes(encoded)

    @property
    def data(self):
        if not self._data:
            self._data = self._convert()

        return self._data

    @property
    def image_file(self):
        return self._img

    @property
    def data_var(self):
        return self._data_var

    @property
    def bank_var(self):
        return self._bank_var

    @property
    def bank_num(self):
        return self._bank_num

    def __len__(self):
        return len(self.data)

    def __str__(self):
        return 'Tileset ' + self.image_file
