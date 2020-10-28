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
 * KMS/DRM Video Backend for SDL 1.2.x
 * Copyright (C) 2020 Paul Cercueil <paul@crapouillou.net>
 * Copyright (C) 2020 João H. Spies <johnnyonflame@hotmail.com>
 *
 * Rætro and SDLite 1.2.x:
 * Copyright (c) 2019-2020 Marcus Andrade <marcus@raetro.org>
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

#include "SDL_kmsdrmvideo.h"

#ifndef _SDL_kmsdrmmisc_h
#define _SDL_kmsdrmmisc_h

void dump_mode(drmModeModeInfo *mode);

int save_drm_pipe(_THIS, Uint32 plane, Uint32 crtc, Uint32 enc, drmModeConnector *conn);

Uint32 get_prop_id(_THIS, Uint32 obj_id, const char *prop_name);

int acquire_properties(_THIS, Uint32 id, Uint32 type);

int get_property(_THIS, uint32_t obj_id, const char *name, uint64_t *value);

int add_property(_THIS, drmModeAtomicReq *req, uint32_t obj_id, const char *name, int opt, uint64_t value);

int find_property(_THIS, uint32_t obj_id, const char *name);

int free_drm_prop_storage(_THIS);

int free_drm_pipe(_THIS);

drmModeModeInfo *find_pipe_closest_refresh(drm_pipe *pipe, float refresh);

#endif /* _SDL_kmsdrmmisc_h */
