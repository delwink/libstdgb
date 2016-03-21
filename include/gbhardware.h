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

#define GB_RAM             ((uint8_t *) 0xC000)
#define GB_RAM_END         ((uint8_t *) 0xDFFF)

#define GB_OAMRAM          ((uint8_t *) 0xFE00)
#define GB_OAMRAM_END      ((uint8_t *) 0xFE9F)

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

#define GB_LCD_YPOS        ((uint8_t *) 0xFF44)
#define GB_LCD_XPOS        ((uint8_t *) 0xFF45)

#define GB_DMA             ((uint8_t *) 0xFF46)

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

/* MBC1 controls */

#define GB_MBC1_MODEL_16_8 (0x00)
#define GB_MBC1_MODEL_4_32 (0x01)

#define gb_mbc1_set_model(M) *((uint8_t *) 0x6000) = (M);

/* MBC5 controls */

#define gb_mbc5_select_rombank(B)				\
  {								\
    *((uint8_t *) 0x2100) = (uint8_t) ((B) & 0x00FF);		\
    *((uint8_t *) 0x3000) = (uint8_t) (((B) & 0x0100) >> 8);	\
  }

/* generic controls */

#define gb_disable_interrupts() __asm__ ("di");
#define gb_enable_interrupts() __asm__ ("ei");
#define gb_halt() __asm__ ("halt\n\tnop");
#define gb_stop() __asm__ ("stop");

#define gb_enable_rambank() *((uint8_t *) 0x0000) = 0x0A;
#define gb_disable_rambank() *((uint8_t *) 0x0000) = 0x00;

#define gb_select_rombank(B) *((uint8_t *) 0x2100) = (B);
#define gb_select_rambank(B) *((uint8_t *) 0x4000) = (B);

#endif
