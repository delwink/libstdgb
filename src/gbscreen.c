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

#include <stdbool.h>

#include "gbhardware.h"
#include "gbscreen.h"

uint8_t _gb_char_offset = 0;
uint8_t _gb_cursor_x = 0;
uint8_t _gb_cursor_x_limit = GB_LCD_X_BYTE;
uint8_t _gb_cursor_y = 0;
uint8_t _gb_cursor_y_limit = GB_LCD_Y_BYTE;
uint8_t *_gb_active_screen = GB_SCRN0;

static volatile bool vblank = false;
static uint16_t offset = 0;

void
_vblank_trigger ()
{
  __asm__ ("push hl");
  vblank = true;
  __asm__ ("pop hl");
  gb_enable_interrupts ();
}

void
gb_wait_vblank ()
{
  *GB_INT_ENABLE |= GB_INT_VBLANK;
  gb_enable_interrupts ();

  vblank = false;

  while (!vblank)
    gb_halt ();
}

void
gb_define_tile (uint8_t start, uint8_t *data)
{
  uint8_t *tiles = GB_TILE_DATA;
  uint8_t i;

  for (tiles = &tiles[start * 16], i = 16; i != 0; ++tiles, ++data, --i)
    *tiles = *data;
}

void
gb_define_reverse_tile (uint8_t start, uint8_t *data)
{
  uint8_t *tiles = GB_TILE_DATA;
  uint8_t i;

  for (tiles = &tiles[start * 16], i = 16; i != 0; ++tiles, ++data, --i)
    *tiles = ~(*data);
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

void
gb_dec_cursor ()
{
  if (_gb_cursor_x == 0)
    {
      --_gb_cursor_y;
      _gb_cursor_x = _gb_cursor_x_limit - 1;
    }
  else
    {
      --_gb_cursor_x;
    }
}

void
gb_set_view (uint8_t x, uint8_t y)
{
  *GB_SCROLL_X = x;
  *GB_SCROLL_Y = y;

  offset = x / 8;
  offset += 4 * y; // optimized (32 * (y / 8))
}

void
gb_puttile (uint8_t tile)
{
  uint16_t i;

  i = offset;
  i += 32 * _gb_cursor_y;
  i += _gb_cursor_x;

  if ((offset % 32) + _gb_cursor_x >= 32)
    i -= 32;

  i %= 0x400;

  if (!gb_have_vblank ())
    gb_wait_vblank ();

  _gb_active_screen[i] = tile;
  gb_inc_cursor ();
}

/* declared in stdio.h but not implemented in sdcc */
void
putchar (char c)
{
  switch (c)
    {
    case '\b':
      gb_dec_cursor ();
      break;

    case '\n':
      ++_gb_cursor_y;
    case '\r':
      _gb_cursor_x = 0;
      break;

    case '\v':
      ++_gb_cursor_y;
      break;

    default:
      if (c >= 0)
	gb_puttile (c + _gb_char_offset);
    }
}
