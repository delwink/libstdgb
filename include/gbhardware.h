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

#define VRAM            ((uint8_t *) 0x8000)
#define VRAM_END        ((uint8_t *) 0xA000)

#define SCRN0           ((uint8_t *) 0x9800)
#define SCRN0_END       ((uint8_t *) 0x9BFF)

#define SCRN1           ((uint8_t *) 0x9C00)
#define SCRN1_END       ((uint8_t *) 0x9FFF)

#define RAM             ((uint8_t *) 0xC000)
#define RAM_END         ((uint8_t *) 0xE000)

#define OAMRAM          ((uint8_t *) 0xFE00)
#define OAMRAM_END      ((uint8_t *) 0xFE9F)

#define AUD3WAVERAM     ((uint8_t *) 0xFF30)
#define AUD3WAVERAM_END ((uint8_t *) 0xFF3F)

#define HRAM            ((uint8_t *) 0xFF80)
#define HRAM_END        ((uint8_t *) 0xFFFE)

#define JOYPD           ((uint8_t *) 0xFF00)

#define SER_XFER        ((uint8_t *) 0xFF01)
#define SER_IO          ((uint8_t *) 0xFF02)

#define DIVIDER         ((uint8_t *) 0xFF04)

#define TIMER_COUNT     ((uint8_t *) 0xFF05)
#define TIMER_MOD       ((uint8_t *) 0xFF06)
#define TIMER_CTRL      ((uint8_t *) 0xFF07)

#define INTERRUPT       ((uint8_t *) 0xFF0F)

#define LCD             ((uint8_t *) 0xFF40)

#define LCD_STATE       ((uint8_t *) 0xFF41)

#define SCROLL_Y        ((uint8_t *) 0xFF42)
#define SCROLL_X        ((uint8_t *) 0xFF43)

#define LCD_YPOS        ((uint8_t *) 0xFF44)
#define LCD_XPOS        ((uint8_t *) 0xFF45)

#define DMA             ((uint8_t *) 0xFF46)

#define BG_PALETTE      ((uint8_t *) 0xFF47)

#define OBJ_PALETTE0    ((uint8_t *) 0xFF48)
#define OBJ_PALETTE1    ((uint8_t *) 0xFF49)

#define WIN_YPOS        ((uint8_t *) 0xFF4A)
#define WIN_XPOS        ((uint8_t *) 0xFF4B)

#define CPU_SPEED       ((uint8_t *) 0xFF4D)

#define VRAM_BANK       ((uint8_t *) 0xFF4F)

#define HDMA1           ((uint8_t *) 0xFF51)
#define HDMA2           ((uint8_t *) 0xFF52)
#define HDMA3           ((uint8_t *) 0xFF53)
#define HDMA4           ((uint8_t *) 0xFF54)
#define HDMA5           ((uint8_t *) 0xFF55)

#define IR_COMM_PORT    ((uint8_t *) 0xFF56)

#define BG_PAL_SPEC     ((uint8_t *) 0xFF68)
#define BG_PAL_DATA     ((uint8_t *) 0xFF69)

#define OBJ_PAL_SPEC    ((uint8_t *) 0xFF6A)
#define OBJ_PAL_DATA    ((uint8_t *) 0xFF6B)

#define SEL_MAIN_RBANK  ((uint8_t *) 0xFF70)

#define INT_ENABLE      ((uint8_t *) 0xFFFF)

#define AUDIO_VOLUME    ((uint8_t *) 0xFF24)
#define AUDIO_TERMINAL  ((uint8_t *) 0xFF25)
#define AUDIO_ENABLE    ((uint8_t *) 0xFF26)

#define AUD1_SWEEP      ((uint8_t *) 0xFF10)
#define AUD1_LEN        ((uint8_t *) 0xFF11)
#define AUD1_ENV        ((uint8_t *) 0xFF12)
#define AUD1_LOW        ((uint8_t *) 0xFF13)
#define AUD1_HIGH       ((uint8_t *) 0xFF14)

#define AUD2_LEN        ((uint8_t *) 0xFF16)
#define AUD2_ENV        ((uint8_t *) 0xFF17)
#define AUD2_LOW        ((uint8_t *) 0xFF18)
#define AUD2_HIGH       ((uint8_t *) 0xFF19)

#define AUD3_ENABLE     ((uint8_t *) 0xFF1A)
#define AUD3_LEN        ((uint8_t *) 0xFF1B)
#define AUD3_LEVEL      ((uint8_t *) 0xFF1C)
#define AUD3_LOW        ((uint8_t *) 0xFF1D)
#define AUD3_HIGH       ((uint8_t *) 0xFF1E)

#define AUD4_LEN        ((uint8_t *) 0xFF20)
#define AUD4_ENV        ((uint8_t *) 0xFF21)
#define AUD4_POLY       ((uint8_t *) 0xFF22)
#define AUD4_GO         ((uint8_t *) 0xFF23) // strange name for this flag

/* OAM flags */

#define OAM_PRI   (0x80)
#define OAM_YFLIP (0x40)
#define OAM_XFLIP (0x20)
#define OAM_PAL0  (0x00)
#define OAM_PAL1  (0x10)

/* timer control flags */

#define TIMER_START  (0x04)
#define TIMER_STOP   (0x00)
#define TIMER_4KHZ   (0x00)
#define TIMER_16KHZ  (0x03)
#define TIMER_65KHZ  (0x02)
#define TIMER_262KHZ (0x01)

/* LCD flags */

#define LCD_OFF     (0x00)
#define LCD_ON      (0x80)
#define LCD_WIN9800 (0x00)
#define LCD_WIN9C00 (0x40)
#define LCD_WINOFF  (0x00)
#define LCD_WINON   (0x20)
#define LCD_BG8800  (0x00)
#define LCD_BG8000  (0x10)
#define LCD_BG9800  (0x00)
#define LCD_BG9C00  (0x08)
#define LCD_BGOFF   (0x00)
#define LCD_BGON    (0x01)
#define LCD_OBJ8    (0x00)
#define LCD_OBJ16   (0x04)
#define LCD_OBJOFF  (0x00)
#define LCD_OBJON   (0x02)

#define LCD_LYC     (0x40)
#define LCD_MODE10  (0x20)
#define LCD_MODE01  (0x10)
#define LCD_MODE00  (0x08)
#define LCD_LYCF    (0x04)
#define LCD_HBLANK  (0x00)
#define LCD_VBLANK  (0x01)
#define LCD_OAM     (0x02)
#define LCD_VRAM    (0x03)
#define LCD_BUSY    (0x02)

/* interrupt flags */

#define INT_HILO   (0x10)
#define INT_SERIAL (0x08)
#define INT_TIMER  (0x04)
#define INT_LCD    (0x02)
#define INT_VBLANK (0x01)

/* input codes */

#define PAD_DOWN   (0x80)
#define PAD_UP     (0x40)
#define PAD_LEFT   (0x20)
#define PAD_RIGHT  (0x10)

#define BTN_START  (0x08)
#define BTN_SELECT (0x04)
#define BTN_B      (0x02)
#define BTN_A      (0x01)

#endif
