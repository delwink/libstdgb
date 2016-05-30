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

#include "stdgb.h"

uint8_t (* const GB_OBJECTS)[GB_BYTES_PER_OBJ] = (void *) 0xDF00;
static uint8_t * const DPAD_STATE = (void *) 0xDFA0;
static uint8_t * const BTN_STATE = (void *) 0xDFA1;

void
gb_mbc5_select_rombank (uint16_t bank)
{
  *((uint16_t *) 0x2FFF) = bank;
}

static void
invert_state (uint8_t *state)
{
  *state = (~(*state)) & 0x0F;
}

void
gb_update_input_state ()
{
  // this routine is copied from the official Nintendo programming manual
  __asm__ ("ld a, #0x20\n\t"     // load flag to read d-pad input
	   "ld (#0xFF00), a\n\t" // make input request
	   "ld a, (#0xFF00)\n\t" // read input ports
	   "ld a, (#0xFF00)\n\t" // do this twice for necessary delay
	   "ld (#0xDFA0), a\n\t" // save it in *DPAD_STATE

	   "ld a, #0x10\n\t"     // load flag to read button input
	   "ld (#0xFF00), a\n\t" // make input request
	   "ld a, (#0xFF00)\n\t" // read input ports
	   "ld a, (#0xFF00)\n\t" // do this six times for necessary delay
	   "ld a, (#0xFF00)\n\t"
	   "ld a, (#0xFF00)\n\t"
	   "ld a, (#0xFF00)\n\t"
	   "ld a, (#0xFF00)\n\t"
	   "ld (#0xDFA1), a\n\t" // save it in *BTN_STATE

	   "ld a, #0x30\n\t"     // load flag to read nothing
	   "ld (#0xFF00), a");   // disable input reads

  invert_state (DPAD_STATE);
  invert_state (BTN_STATE);
}

uint8_t
gb_dpad_down (uint8_t direction)
{
  return *DPAD_STATE & direction;
}

uint8_t
gb_button_down (uint8_t button)
{
  return *BTN_STATE & button;
}

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
gb_define_tile (uint8_t start, const uint8_t *data)
{
  uint8_t *tiles = GB_TILE_DATA;
  gb_memcpy (&tiles[start * GB_BYTES_PER_TILE], data, GB_BYTES_PER_TILE);
}

void
gb_set_view (uint8_t x, uint8_t y)
{
  *GB_SCROLL_X = x;
  *GB_SCROLL_Y = y;
}

void
gb_shift_view (int8_t x, int8_t y)
{
  *GB_SCROLL_X += x;
  *GB_SCROLL_Y += y;
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
  gb_memcpy (GB_HRAM, (uint8_t *) copy_objects, 12);
  memset (GB_OBJECTS, 0, GB_NUM_OBJECTS * GB_BYTES_PER_OBJ);
  gb_update_objects ();
}

void
gb_update_objects ()
{
  __asm__ ("jp 0xFF80"); // real function is at HRAM
}

void
gb_memcpy (uint8_t *dest, const uint8_t *src, uint8_t n)
{
  while (n--)
    *(dest++) = *(src++);
}
