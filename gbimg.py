#! /usr/bin/env python3
##
##  gbimg - turn PC image files into Game Boy tilesets
##  Copyright (C) 2016 Delwink, LLC
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU Affero General Public License as published by
##  the Free Software Foundation, version 3 only.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Affero General Public License for more details.
##
##  You should have received a copy of the GNU Affero General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
