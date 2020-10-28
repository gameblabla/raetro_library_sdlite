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

#include "SDL_config.h"
#include "SDL_stdinc.h"

#include <libdrm/drm_fourcc.h>

#ifndef _SDL_kmsdrmcolordef_h
#define _SDL_kmsdrmcolordef_h

#define KMSDRM_DEFAULT_COLOR_DEPTH 16
typedef struct drm_color_def {
	Uint32 four_cc;
	Uint32 bpp;
	Uint32 r_mask, g_mask, b_mask, a_mask;
	Uint32 r_bits, g_bits, b_bits, a_bits;
	Uint32 r_shift, g_shift, b_shift, a_shift;
	float h_factor;
} drm_color_def;

/* Must be kept up-to-date with SDL_kmsdrmcolordef.c */
extern drm_color_def KMSDRM_COLOR_C8;
extern drm_color_def KMSDRM_COLOR_XRGB88888;
extern drm_color_def KMSDRM_COLOR_RGB565;
extern drm_color_def KMSDRM_COLOR_XRGB15555;
extern drm_color_def KMSDRM_COLOR_XBGR88888;
extern drm_color_def KMSDRM_COLOR_BGR565;
extern drm_color_def KMSDRM_COLOR_XBGR15555;
extern drm_color_def KMSDRM_COLOR_YUYV;

/* Provides information on how to configure color format. */
drm_color_def *get_drm_color_def(int depth, Uint32 flags);

/* Provides necessary arguments for drm framebuffer creation */
void get_framebuffer_args(const drm_color_def *def, unsigned int handle, unsigned int pitch, Uint16 height, Uint32 *handles, Uint32 *pitches, Uint32 *offsets);

#endif /* _SDL_kmsdrmcolordef_h */
