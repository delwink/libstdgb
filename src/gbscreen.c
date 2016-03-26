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

uint8_t (* const GB_OBJECTS)[GB_BYTES_PER_OBJ] = (void *) 0xDF00;

void
gb_enable_vblank ()
{
  *GB_INT_ENABLE |= GB_INT_VBLANK;
  gb_enable_interrupts ();
}

void
gb_wait_vblank ()
{
  do
    {
      gb_halt ();
    }
  while (!gb_have_vblank ());
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
gb_set_view (uint8_t x, uint8_t y)
{
  *GB_SCROLL_X = x;
  *GB_SCROLL_Y = y;
}

static void
copy_objects ()
{
  __asm__ ("ld a,#0xDF\n\t"        // specify that objects are at 0xDF00
	   "ld (0xFF46),a\n\t"     // start DMA copy
	   "ld a,#0x28\n"          // set wait counter to 40
	   "obj_copy_wait:\n\t"
	   "dec a\n\t"             // decrement counter
	   "jr nz,obj_copy_wait"); // continue to wait until counter is zero
}

void
gb_init_objects ()
{
  const uint8_t *src = (uint8_t *) copy_objects;
  uint8_t *dest = GB_HRAM;
  uint8_t len = 12;

  while (len--)
    *(dest++) = *(src++);

  memset (GB_OAMRAM, 0x00, 0xA0);
}

void
gb_update_objects ()
{
  __asm__ ("jp 0xFF80"); // real function is at HRAM
}

