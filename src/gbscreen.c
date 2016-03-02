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

void
gb_set_lcd_mode (uint8_t mode)
{
  *GB_LCD = mode;
}

void
gb_set_bg_palette (uint8_t palette)
{
  *GB_BG_PALETTE = palette;
}

void
gb_set_all_tile_data (uint8_t pattern)
{
  uint8_t *p;

  for (p = GB_TILE_DATA; p <= GB_TILE_DATA_END; ++p)
    *p = pattern;
}

void
gb_set_all_tile_maps (uint8_t tile)
{
  uint8_t *p;

  for (p = GB_SCRN0; p <= GB_SCRN0_END; ++p)
    *p = tile;
}
