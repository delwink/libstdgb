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

#ifndef DELWINK_GBSOUND_H
#define DELWINK_GBSOUND_H

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

#endif
