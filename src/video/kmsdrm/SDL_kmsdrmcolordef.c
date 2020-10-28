/*******************************************************************************
 * Library       : SDLite 1.2.x
 * Purpose       : Low-level access to a framebuffer, audio output and HID.
 * Module        : Core
 * Project       : Redux for Embedded System
 * Description   : Stripped-down and optimized libraries for RISC processors
 * License       : GNU General Public License v3.0
 *******************************************************************************
 *
 * KMS/DRM Video Backend for SDL 1.2.x
 * Copyright (C) 2020 Paul Cercueil <paul@crapouillou.net>
 * Copyright (C) 2020 João H. Spies <johnnyonflame@hotmail.com>
 *
 * TinyRetroLabs and SDLite 1.2.x:
 * Copyright (c) 2019-2020 Marcus Andrade <boogermann@tinyretrolabs.org>
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

#include <libdrm/drm_fourcc.h>

#include "SDL_video.h"
#include "SDL_kmsdrmcolordef.h"

#define MAKE_RGBA(type, bpp, rbits, gbits, bbits, abits, rsh, gsh, bsh, ash) \
    drm_color_def KMSDRM_COLOR_##type = { \
        DRM_FORMAT_##type, bpp, \
        (0xFF >> (8-rbits)) << rsh, (0xFF >> (8-gbits)) << gsh, \
        (0xFF >> (8-bbits)) << bsh, (0xFF >> (8-abits)) << ash, \
        rbits, gbits, bbits, abits, rsh, gsh, bsh, ash, 1 \
    }

/* Must be kept up-to-date with SDL_kmsdrmcolordef.h */
/*       |   CODE |BPP| R| G| B| A| RS| GS| BS| AS| */
MAKE_RGBA(C8, 8, 8, 8, 8, 0, 0, 0, 0, 0);
MAKE_RGBA(XRGB8888, 32, 8, 8, 8, 0, 16, 8, 0, 0);
MAKE_RGBA(RGB565, 16, 5, 6, 5, 0, 11, 5, 0, 0);
MAKE_RGBA(XRGB1555, 16, 5, 5, 5, 0, 10, 5, 0, 0);
MAKE_RGBA(XBGR8888, 32, 8, 8, 8, 0, 0, 8, 16, 0);
MAKE_RGBA(BGR565, 16, 5, 6, 5, 0, 0, 5, 11, 0);
MAKE_RGBA(XBGR1555, 16, 5, 5, 5, 0, 0, 5, 10, 0);

/** 
 * TODO:: Figure out if there's any extra information that would be useful to
 * have in this macro. 
 **/
#define MAKE_YUV(type, bpp, hf) \
    drm_color_def KMSDRM_COLOR_##type = { \
       DRM_FORMAT_##type, bpp, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, hf \
    };

/* Must be kept up-to-date with SDL_kmsdrmcolordef.h */
MAKE_YUV(YUV444, 8, 3);

/* Provides information on how to configure color format. */
drm_color_def *get_drm_color_def(int depth, Uint32 flags) {
	if(flags & SDL_YUV444) {
		switch (depth) {
			case 24:
				return &KMSDRM_COLOR_YUV444;
			default:
				return NULL;
		}
	} else if(flags & SDL_SWIZZLEBGR) {
		switch (depth) {
			/* case  8: return &KMSDRM_COLOR_YUV444; */
			case 16:
				return &KMSDRM_COLOR_BGR565;
			case 15:
				return &KMSDRM_COLOR_XBGR1555;
			case 24:
			case 32:
				return &KMSDRM_COLOR_XBGR8888;
			default:
				return NULL;
		}
	} else {
		switch (depth) {
			case 8:
				return &KMSDRM_COLOR_C8;
			case 16:
				return &KMSDRM_COLOR_RGB565;
			case 15:
				return &KMSDRM_COLOR_XRGB1555;
			case 24:
			case 32:
				return &KMSDRM_COLOR_XRGB8888;
			default:
				return NULL;
		}
	}
}

/* Provides necessary arguments for drm framebuffer creation */
void get_framebuffer_args(const drm_color_def *def, unsigned int handle, unsigned int pitch, Uint16 height, Uint32 *handles, Uint32 *pitches, Uint32 *offsets) {
	switch (def->four_cc) {
		case DRM_FORMAT_YUV444:
			pitches[0] = pitches[1] = pitches[2] = pitch;
			handles[0] = handles[1] = handles[2] = handle;

			offsets[0] = 0;
			offsets[1] = offsets[0] + pitch * height;
			offsets[2] = offsets[1] + pitch * height;
			break;
		case DRM_FORMAT_C8:
		case DRM_FORMAT_RGB565:
		case DRM_FORMAT_XRGB1555:
		case DRM_FORMAT_XRGB8888:
		case DRM_FORMAT_BGR565:
		case DRM_FORMAT_XBGR1555:
		case DRM_FORMAT_XBGR8888:
			handles[0] = handle;
			pitches[0] = pitch;
			break;
	}
}
