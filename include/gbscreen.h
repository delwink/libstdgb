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
#include <stdlib.h>
#include <string.h>

#define GB_TILE_DATA       ((uint8_t *) 0x8000)
#define GB_TILE_DATA_END   ((uint8_t *) 0x8FFF)

#define GB_SCRN0           ((uint8_t *) 0x9800)
#define GB_SCRN0_END       ((uint8_t *) 0x9BFF)

#define GB_SCRN1           ((uint8_t *) 0x9C00)
#define GB_SCRN1_END       ((uint8_t *) 0x9FFF)

#define GB_LCD_STATE       ((uint8_t *) 0xFF41)

#define GB_SCROLL_Y        ((uint8_t *) 0xFF42)
#define GB_SCROLL_X        ((uint8_t *) 0xFF43)

#define GB_BG_PALETTE      ((uint8_t *) 0xFF47)

#define GB_OBJ_PALETTE0    ((uint8_t *) 0xFF48)
#define GB_OBJ_PALETTE1    ((uint8_t *) 0xFF49)

#define GB_WIN_YPOS        ((uint8_t *) 0xFF4A)
#define GB_WIN_XPOS        ((uint8_t *) 0xFF4B)

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

/* OAM flags */

#define GB_OBJ_PRI   (0x80)
#define GB_OBJ_YFLIP (0x40)
#define GB_OBJ_XFLIP (0x20)
#define GB_OBJ_PAL0  (0x00)
#define GB_OBJ_PAL1  (0x10)

void
gb_set_view (uint8_t x, uint8_t y);

#define gb_shift_view(X,Y)				\
  gb_set_view (*GB_SCROLL_X + (X), *GB_SCROLL_Y + (Y));

#define gb_have_vblank() ((*GB_LCD_STATE & 0x03) == 1)

void
gb_enable_vblank (void);

void
gb_wait_vblank (void);

void
gb_define_tile (uint8_t i, const uint8_t *tile_data);

void
gb_define_reverse_tile (uint8_t i, const uint8_t *tile_data);

#define gb_set_lcd_mode(M) *GB_LCD = (M);

#define gb_set_bg_palette(P) *GB_BG_PALETTE = (P);

#define gb_set_all_tile_data(P) memset (GB_TILE_DATA, (P), 0x1000);
#define gb_set_all_tile_maps(T) memset (GB_SCRN0, (T), 0x800);

#define GB_NUM_OBJECTS (40)
enum _gb_sprite_model
  {
    GB_OBJ_YPOS = 0,
    GB_OBJ_XPOS,
    GB_OBJ_TILE,
    GB_OBJ_FLAGS,
    GB_BYTES_PER_OBJ
  };

extern uint8_t (* const GB_OBJECTS)[GB_BYTES_PER_OBJ];

void
gb_init_objects (void);

void
gb_update_objects (void);

#endif
