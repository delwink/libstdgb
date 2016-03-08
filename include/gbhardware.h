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

#ifndef DELWINK_GBHARDWARE_H
#define DELWINK_GBHARDWARE_H

#include <stdint.h>

/* memory locations */

#define GB_VRAM            ((uint8_t *) 0x8000)
#define GB_VRAM_END        ((uint8_t *) 0x9FFF)

#define GB_TILE_DATA       ((uint8_t *) 0x8000)
#define GB_TILE_DATA_END   ((uint8_t *) 0x8FFF)

#define GB_SCRN0           ((uint8_t *) 0x9800)
#define GB_SCRN0_END       ((uint8_t *) 0x9BFF)

#define GB_SCRN1           ((uint8_t *) 0x9C00)
#define GB_SCRN1_END       ((uint8_t *) 0x9FFF)

#define GB_RAM             ((uint8_t *) 0xC000)
#define GB_RAM_END         ((uint8_t *) 0xDFFF)

#define GB_OAMRAM          ((uint8_t *) 0xFE00)
#define GB_OAMRAM_END      ((uint8_t *) 0xFE9F)

#define GB_AUD3WAVERAM     ((uint8_t *) 0xFF30)
#define GB_AUD3WAVERAM_END ((uint8_t *) 0xFF3F)

#define GB_HRAM            ((uint8_t *) 0xFF80)
#define GB_HRAM_END        ((uint8_t *) 0xFFFE)

#define GB_JOYPD           ((uint8_t *) 0xFF00)

#define GB_SER_XFER        ((uint8_t *) 0xFF01)
#define GB_SER_IO          ((uint8_t *) 0xFF02)

#define GB_DIVIDER         ((uint8_t *) 0xFF04)

#define GB_TIMER_COUNT     ((uint8_t *) 0xFF05)
#define GB_TIMER_MOD       ((uint8_t *) 0xFF06)
#define GB_TIMER_CTRL      ((uint8_t *) 0xFF07)

#define GB_INTERRUPT       ((uint8_t *) 0xFF0F)

#define GB_LCD             ((uint8_t *) 0xFF40)

#define GB_LCD_STATE       ((uint8_t *) 0xFF41)

#define GB_SCROLL_Y        ((uint8_t *) 0xFF42)
#define GB_SCROLL_X        ((uint8_t *) 0xFF43)

#define GB_LCD_YPOS        ((uint8_t *) 0xFF44)
#define GB_LCD_XPOS        ((uint8_t *) 0xFF45)

#define GB_DMA             ((uint8_t *) 0xFF46)

#define GB_BG_PALETTE      ((uint8_t *) 0xFF47)

#define GB_OBJ_PALETTE0    ((uint8_t *) 0xFF48)
#define GB_OBJ_PALETTE1    ((uint8_t *) 0xFF49)

#define GB_WIN_YPOS        ((uint8_t *) 0xFF4A)
#define GB_WIN_XPOS        ((uint8_t *) 0xFF4B)

#define GB_CPU_SPEED       ((uint8_t *) 0xFF4D)

#define GB_VRAM_BANK       ((uint8_t *) 0xFF4F)

#define GB_HDMA1           ((uint8_t *) 0xFF51)
#define GB_HDMA2           ((uint8_t *) 0xFF52)
#define GB_HDMA3           ((uint8_t *) 0xFF53)
#define GB_HDMA4           ((uint8_t *) 0xFF54)
#define GB_HDMA5           ((uint8_t *) 0xFF55)

#define GB_IR_COMM_PORT    ((uint8_t *) 0xFF56)

#define GB_BG_PAL_SPEC     ((uint8_t *) 0xFF68)
#define GB_BG_PAL_DATA     ((uint8_t *) 0xFF69)

#define GB_OBJ_PAL_SPEC    ((uint8_t *) 0xFF6A)
#define GB_OBJ_PAL_DATA    ((uint8_t *) 0xFF6B)

#define GB_SEL_MAIN_RBANK  ((uint8_t *) 0xFF70)

#define GB_INT_ENABLE      ((uint8_t *) 0xFFFF)

#define GB_AUDIO_VOLUME    ((uint8_t *) 0xFF24)
#define GB_AUDIO_TERMINAL  ((uint8_t *) 0xFF25)
#define GB_AUDIO_ENABLE    ((uint8_t *) 0xFF26)

#define GB_AUD1_SWEEP      ((uint8_t *) 0xFF10)
#define GB_AUD1_LEN        ((uint8_t *) 0xFF11)
#define GB_AUD1_ENV        ((uint8_t *) 0xFF12)
#define GB_AUD1_LOW        ((uint8_t *) 0xFF13)
#define GB_AUD1_HIGH       ((uint8_t *) 0xFF14)

#define GB_AUD2_LEN        ((uint8_t *) 0xFF16)
#define GB_AUD2_ENV        ((uint8_t *) 0xFF17)
#define GB_AUD2_LOW        ((uint8_t *) 0xFF18)
#define GB_AUD2_HIGH       ((uint8_t *) 0xFF19)

#define GB_AUD3_ENABLE     ((uint8_t *) 0xFF1A)
#define GB_AUD3_LEN        ((uint8_t *) 0xFF1B)
#define GB_AUD3_LEVEL      ((uint8_t *) 0xFF1C)
#define GB_AUD3_LOW        ((uint8_t *) 0xFF1D)
#define GB_AUD3_HIGH       ((uint8_t *) 0xFF1E)

#define GB_AUD4_LEN        ((uint8_t *) 0xFF20)
#define GB_AUD4_ENV        ((uint8_t *) 0xFF21)
#define GB_AUD4_POLY       ((uint8_t *) 0xFF22)
#define GB_AUD4_GO         ((uint8_t *) 0xFF23) // strange name for this flag

/* OAM flags */

#define GB_OAM_PRI   (0x80)
#define GB_OAM_YFLIP (0x40)
#define GB_OAM_XFLIP (0x20)
#define GB_OAM_PAL0  (0x00)
#define GB_OAM_PAL1  (0x10)

/* timer control flags */

#define GB_TIMER_START  (0x04)
#define GB_TIMER_STOP   (0x00)
#define GB_TIMER_4KHZ   (0x00)
#define GB_TIMER_16KHZ  (0x03)
#define GB_TIMER_65KHZ  (0x02)
#define GB_TIMER_262KHZ (0x01)

/* interrupt flags */

#define GB_INT_HILO   (0x10)
#define GB_INT_SERIAL (0x08)
#define GB_INT_TIMER  (0x04)
#define GB_INT_LCD    (0x02)
#define GB_INT_VBLANK (0x01)

/* input codes */

#define GB_PAD_DOWN   (0x80)
#define GB_PAD_UP     (0x40)
#define GB_PAD_LEFT   (0x20)
#define GB_PAD_RIGHT  (0x10)

#define GB_BTN_START  (0x08)
#define GB_BTN_SELECT (0x04)
#define GB_BTN_B      (0x02)
#define GB_BTN_A      (0x01)

#define gb_disable_interrupts() __asm__ ("di");
#define gb_enable_interrupts() __asm__("ei");

#define gb_set_rombank(B) *((uint8_t *) 0x2000) = (B);

#endif
