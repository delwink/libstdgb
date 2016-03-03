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

#ifndef DELWINK_GBSCREEN_H
#define DELWINK_GBSCREEN_H

#include <stdint.h>

/* LCD flags */

#define GB_LCD_OFF     (0x00)
#define GB_LCD_ON      (0x80)
#define GB_LCD_WIN9800 (0x00)
#define GB_LCD_WIN9C00 (0x40)
#define GB_LCD_WINOFF  (0x00)
#define GB_LCD_WINON   (0x20)
#define GB_LCD_BG8800  (0x00)
#define GB_LCD_BG8000  (0x10)
#define GB_LCD_BG9800  (0x00)
#define GB_LCD_BG9C00  (0x08)
#define GB_LCD_BGOFF   (0x00)
#define GB_LCD_BGON    (0x01)
#define GB_LCD_OBJ8    (0x00)
#define GB_LCD_OBJ16   (0x04)
#define GB_LCD_OBJOFF  (0x00)
#define GB_LCD_OBJON   (0x02)

#define GB_LCD_LYC     (0x40)
#define GB_LCD_MODE10  (0x20)
#define GB_LCD_MODE01  (0x10)
#define GB_LCD_MODE00  (0x08)
#define GB_LCD_LYCF    (0x04)
#define GB_LCD_HBLANK  (0x00)
#define GB_LCD_VBLANK  (0x01)
#define GB_LCD_OAM     (0x02)
#define GB_LCD_VRAM    (0x03)
#define GB_LCD_BUSY    (0x02)

/* screen facts */

#define GB_LCD_X_SIZE  (160)
#define GB_LCD_Y_SIZE  (144)
#define GB_LCD_X_BYTE  (20)
#define GB_LCD_Y_BYTE  (18)

extern uint8_t _gb_char_offset;

#define gb_set_ascii_offset(N) _gb_char_offset = N;

extern uint8_t _gb_cursor_x, _gb_cursor_y;

#define gb_set_cursor(X,Y) _gb_cursor_x = X; _gb_cursor_y = Y;

extern uint8_t _gb_cursor_x_limit, _gb_cursor_y_limit;

#define gb_set_cursor_x_limit(L) _gb_cursor_x_limit = L;
#define gb_set_cursor_y_limit(L) _gb_cursor_y_limit = L;

void
gb_inc_cursor (void);

void
gb_set_view (uint8_t x, uint8_t y);

#define gb_shift_view(X,Y) gb_set_view (*GB_SCROLL_X + (X), *GB_SCROLL_Y + (Y));

#define gb_define_tile(I,T) *(GB_TILE_DATA + (I)) = T;

void
gb_puttile (uint8_t tile);

void
gb_set_lcd_mode (uint8_t mode);

void
gb_set_bg_palette (uint8_t palette);

void
gb_set_all_tile_data (uint8_t pattern);

void
gb_set_all_tile_maps (uint8_t tile);

#endif
