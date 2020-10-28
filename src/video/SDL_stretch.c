// SPDX-License-Identifier: GPL-3.0-or-later
/*******************************************************************************
 * Library       : SDLite 1.2.x
 * Purpose       : Low-level access to a framebuffer, audio output and HID.
 * Module        : Core
 * Project       : Redux for Embedded System
 * Description   : Stripped-down and optimized libraries for RISC processors
 * License       : GNU General Public License v3.0
 *******************************************************************************
 *
 * Rætro and SDLite 1.2.x:
 * Copyright (c) 2019-2020 Marcus Andrade <marcus@raetro.org>
 *
 * Stretch Blit Implementation:
 * Copyright (c) 1997 Tomasz Cejner
 *
 * Simple DirectMedia Layer and SDL:
 * Copyright (c) 1997-2012 Sam Lantinga <slouken@libsdl.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.
 *
 ******************************************************************************/
#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_blit.h"

/**
 * This isn't ready for general consumption yet - it should be folded into the general blitting mechanism.
 */

#define DEFINE_COPY_ROW(name, type) \
void name(type *src, int src_w, type *dst, int dst_w) { \
    int i;                          \
    int pos, inc;                   \
    type pixel = 0;                 \
                                    \
    pos = 0x10000;                  \
    inc = (src_w << 16) / dst_w;    \
    for ( i=dst_w; i>0; --i ) {     \
        while ( pos >= 0x10000L ) { \
            pixel = *src++;         \
            pos -= 0x10000L;        \
        }                           \
        *dst++ = pixel;             \
        pos += inc;                 \
    }                               \
}

DEFINE_COPY_ROW(copy_row1, Uint8)

DEFINE_COPY_ROW(copy_row2, Uint16)

DEFINE_COPY_ROW(copy_row4, Uint32)

/** The ASM code doesn't handle 24-bpp stretch blits */
void copy_row3(Uint8 *src, int src_w, Uint8 *dst, int dst_w) {
	int i;
	int pos, inc;
	Uint8 pixel[3] = {
		0,
		0,
		0
	};

	pos = 0x10000;
	inc = (src_w << 16) / dst_w;
	for (i = dst_w; i > 0; --i) {
		while (pos >= 0x10000L) {
			pixel[0] = *src++;
			pixel[1] = *src++;
			pixel[2] = *src++;
			pos -= 0x10000L;
		}
		*dst++ = pixel[0];
		*dst++ = pixel[1];
		*dst++ = pixel[2];
		pos += inc;
	}
}

/* Perform a stretch blit between two surfaces of the same format.
   NOTE:  This function is not safe to call from multiple threads!
*/
int SDL_SoftStretch(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, const SDL_Rect *dstrect) {
	int src_locked;
	int dst_locked;
	int pos, inc;
	int dst_maxrow;
	int src_row, dst_row;
	Uint8 *srcp = NULL;
	Uint8 *dstp;
	SDL_Rect full_src;
	SDL_Rect full_dst;

	const int bpp = dst->format->BytesPerPixel;

	if(src->format->format != dst->format->format) {
		SDL_SetError("Only works with same format surfaces");
		return -1;
	}

	/* Verify the blit rectangles */
	if(srcrect) {
		if((srcrect->x < 0) || (srcrect->y < 0) || ((srcrect->x + srcrect->w) > src->w) || ((srcrect->y + srcrect->h) > src->h)) {
			SDL_SetError("Invalid source blit rectangle");
			return -1;
		}
	} else {
		full_src.x = 0;
		full_src.y = 0;
		full_src.w = src->w;
		full_src.h = src->h;
		srcrect = &full_src;
	}
	if(dstrect) {
		if((dstrect->x < 0) || (dstrect->y < 0) || ((dstrect->x + dstrect->w) > dst->w) || ((dstrect->y + dstrect->h) > dst->h)) {
			SDL_SetError("Invalid destination blit rectangle");
			return -1;
		}
	} else {
		full_dst.x = 0;
		full_dst.y = 0;
		full_dst.w = dst->w;
		full_dst.h = dst->h;
		dstrect = &full_dst;
	}

	/* Lock the destination if it's in hardware */
	dst_locked = 0;
	if(SDL_MUSTLOCK(dst)) {
		if(SDL_LockSurface(dst) < 0) {
			SDL_SetError("Unable to lock destination surface");
			return -1;
		}
		dst_locked = 1;
	}
	/* Lock the source if it's in hardware */
	src_locked = 0;
	if(SDL_MUSTLOCK(src)) {
		if(SDL_LockSurface(src) < 0) {
			if(dst_locked) {
				SDL_UnlockSurface(dst);
			}
			SDL_SetError("Unable to lock source surface");
			return -1;
		}
		src_locked = 1;
	}

	/* Set up the data... */
	pos = 0x10000;
	inc = (srcrect->h << 16) / dstrect->h;
	src_row = srcrect->y;
	dst_row = dstrect->y;

	/* Perform the stretch blit */
	for (dst_maxrow = dst_row + dstrect->h; dst_row < dst_maxrow; ++dst_row) {
		dstp = (Uint8 *) dst->pixels + (dst_row * dst->pitch) + (dstrect->x * bpp);
		while (pos >= 0x10000L) {
			srcp = (Uint8 *) src->pixels + (src_row * src->pitch) + (srcrect->x * bpp);
			++src_row;
			pos -= 0x10000L;
		}
		switch (bpp) {
			case 1:
				copy_row1(srcp, srcrect->w, dstp, dstrect->w);
				break;
			case 2:
				copy_row2((Uint16 *) srcp, srcrect->w, (Uint16 *) dstp, dstrect->w);
				break;
			case 3:
				copy_row3(srcp, srcrect->w, dstp, dstrect->w);
				break;
			case 4:
				copy_row4((Uint32 *) srcp, srcrect->w, (Uint32 *) dstp, dstrect->w);
				break;
		}
		pos += inc;
	}

	/* We need to unlock the surfaces if they're locked */
	if(dst_locked) {
		SDL_UnlockSurface(dst);
	}
	if(src_locked) {
		SDL_UnlockSurface(src);
	}
	return (0);
}
