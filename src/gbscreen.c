/*
 *  libstdgb - library of useful Game Boy operations
 *  Copyright (C) 2016 Delwink, LLC
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, version 3 only.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gbhardware.h"
#include "gbscreen.h"

uint8_t _gb_char_offset = 0;
uint8_t _gb_cursor_x = 0;
uint8_t _gb_cursor_x_limit = GB_LCD_X_BYTE;
uint8_t _gb_cursor_y = 0;
uint8_t _gb_cursor_y_limit = GB_LCD_Y_BYTE;

static size_t offset = 0;

void
gb_define_tile (size_t start, uint8_t *data)
{
  uint8_t *tiles = GB_TILE_DATA;
  size_t i;

  for (tiles = &tiles[start * 16], i = 0; i < 16; ++i)
    tiles[i] = data[i];
}

void
gb_inc_cursor ()
{
  if (++_gb_cursor_x >= _gb_cursor_x_limit)
    {
      _gb_cursor_x = 0;
      if (++_gb_cursor_y >= _gb_cursor_y_limit)
	_gb_cursor_y = 0;
    }
}

static uint8_t
get_byte_offset (uint8_t shift)
{
  return shift / 8;
}

void
gb_set_view (uint8_t x, uint8_t y)
{
  *GB_SCROLL_X = x;
  *GB_SCROLL_Y = y;

  offset = get_byte_offset (x);
  offset += 32 * get_byte_offset (y);
}

void
gb_puttile (uint8_t tile)
{
  uint8_t *tiles = GB_SCRN0;
  size_t i;

  i = offset;
  i += 32 * _gb_cursor_y;
  i += _gb_cursor_x;

  tiles[i] = tile;
  gb_inc_cursor ();
}

/* declared in stdio.h but not implemented in sdcc */
void
putchar (char c)
{
  if (c >= 0)
    gb_puttile (c);
}
