/*
 *  libstdgb - library of useful Game Boy operations
 *  Copyright (C) 2016-2017 Delwink, LLC
 *
 *  Redistributions, modified or unmodified, in whole or in part, must retain
 *  applicable notices of copyright or other legal privilege, these conditions,
 *  and the following license terms and disclaimer.  Subject to these
 *  conditions, each holder of copyright or other legal privileges, author or
 *  assembler, and contributor of this work, henceforth "licensor", hereby
 *  grants to any person who obtains a copy of this work in any form:
 *
 *  1. Permission to reproduce, modify, distribute, publish, sell, sublicense,
 *  use, and/or otherwise deal in the licensed material without restriction.
 *
 *  2. A perpetual, worldwide, non-exclusive, royalty-free, gratis, irrevocable
 *  patent license to make, have made, provide, transfer, import, use, and/or
 *  otherwise deal in the licensed material without restriction, for any and
 *  all patents held by such licensor and necessarily infringed by the form of
 *  the work upon distribution of that licensor's contribution to the work
 *  under the terms of this license.
 *
 *  NO WARRANTY OF ANY KIND IS IMPLIED BY, OR SHOULD BE INFERRED FROM, THIS
 *  LICENSE OR THE ACT OF DISTRIBUTION UNDER THE TERMS OF THIS LICENSE,
 *  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *  A PARTICULAR PURPOSE, AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS,
 *  ASSEMBLERS, OR HOLDERS OF COPYRIGHT OR OTHER LEGAL PRIVILEGE BE LIABLE FOR
 *  ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN ACTION OF CONTRACT,
 *  TORT, OR OTHERWISE ARISING FROM, OUT OF, OR IN CONNECTION WITH THE WORK OR
 *  THE USE OF OR OTHER DEALINGS IN THE WORK.
 */

#ifndef DELWINK_STDGB_H
#define DELWINK_STDGB_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* memory locations */

#define GB_VRAM            ((uint8_t *) 0x8000)
#define GB_VRAM_END        ((uint8_t *) 0x9FFF)

#define GB_RAM             ((uint8_t *) 0xC000)
#define GB_RAM_END         ((uint8_t *) 0xDFFF)

#define GB_OAMRAM          ((uint8_t *) 0xFE00)
#define GB_OAMRAM_END      ((uint8_t *) 0xFE9F)

#define GB_HRAM            ((uint8_t *) 0xFF80)
#define GB_HRAM_END        ((uint8_t *) 0xFFFE)

#define GB_INPUTS          ((uint8_t *) 0xFF00)

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

/* joypad/button input */

#define GB_DPAD_DOWN   (0x08)
#define GB_DPAD_UP     (0x04)
#define GB_DPAD_LEFT   (0x02)
#define GB_DPAD_RIGHT  (0x01)

#define GB_BTN_START  (0x08)
#define GB_BTN_SELECT (0x04)
#define GB_BTN_B      (0x02)
#define GB_BTN_A      (0x01)

void
gb_update_input_state(void);

uint8_t
gb_dpad_down(uint8_t direction);

uint8_t
gb_button_down(uint8_t button);

/* MBC1 controls */

#define GB_MBC1_MODEL_16_8 (0x00)
#define GB_MBC1_MODEL_4_32 (0x01)

#define gb_mbc1_set_model(M) *((uint8_t *) 0x6000) = (M);

/* MBC5 controls */

void
gb_mbc5_select_rombank(uint16_t bank);

/* generic controls */

#define gb_disable_interrupts() __asm__("di");
#define gb_enable_interrupts() __asm__("ei");
#define gb_halt() __asm__("halt\n\tnop");
#define gb_stop() __asm__("stop");

#define gb_enable_rambank() *((uint8_t *) 0x0000) = 0x0A;
#define gb_disable_rambank() *((uint8_t *) 0x0000) = 0x00;

#define gb_select_rombank(B) *((uint8_t *) 0x2100) = (B);
#define gb_select_rambank(B) *((uint8_t *) 0x4000) = (B);

/* screen control */

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
#define GB_OBJ_PALETTES    ((uint8_t *) 0xFF48)

#define GB_WIN_YPOS        ((uint8_t *) 0xFF4A)
#define GB_WIN_XPOS        ((uint8_t *) 0xFF4B)

/* LCD flags */

#define GB_LCD_OFF      (0x00)
#define GB_LCD_ON       (0x80)
#define GB_LCD_WINSCRN1 (0x40)
#define GB_LCD_WINSCRN0 (0x20)
#define GB_LCD_BGON     (0x11)
#define GB_LCD_OBJ16    (0x04)
#define GB_LCD_OBJ8     (0x02)

#define GB_LCD_LYC      (0x40)
#define GB_LCD_MODE10   (0x20)
#define GB_LCD_MODE01   (0x10)
#define GB_LCD_MODE00   (0x08)
#define GB_LCD_LYCF     (0x04)
#define GB_LCD_HBLANK   (0x00)
#define GB_LCD_VBLANK   (0x01)
#define GB_LCD_OAM      (0x02)
#define GB_LCD_VRAM     (0x03)
#define GB_LCD_BUSY     (0x02)

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
gb_set_view(uint8_t x, uint8_t y);

void
gb_shift_view(int8_t x, int8_t y);

#define gb_have_vblank() ((*GB_LCD_STATE & 0x03) == 1)

void
gb_enable_vblank(void);

void
gb_wait_vblank(void);

#define GB_BYTES_PER_TILE (16)

void
gb_define_tile(uint8_t i, const uint8_t *tile_data);

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
gb_init_objects(void);

void
gb_update_objects(void);

#define gb_set_object_palette(I,P) GB_OBJ_PALETTES[(I)] = (P);

/* audio control */

#define GB_AUD3WAVERAM     ((uint8_t *) 0xFF30)
#define GB_AUD3WAVERAM_END ((uint8_t *) 0xFF3F)

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

/* generic functions */

void
gb_memcpy(uint8_t *dest, const uint8_t *src, uint8_t n);

#endif
