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

#ifndef SDL_dspaudio_h_
#define SDL_dspaudio_h_

#include "../SDL_sysaudio.h"

/* Hidden "this" pointer for the video functions */
#define _THIS    SDL_AudioDevice *this

struct SDL_PrivateAudioData {
	/* The file descriptor for the audio device */
	int audio_fd;

	/* The parent process id, to detect when application quits */
	pid_t parent;

	/* Raw mixing buffer */
	Uint8 *dma_buf;
	int dma_len;
	int num_buffers;

	/* Support for audio timing using a timer, in addition to select() */
	float frame_ticks;
	float next_frame;
};
#define FUDGE_TICKS    10    /* The scheduler overhead ticks per frame */

/* Old variable names */
#define audio_fd        (this->hidden->audio_fd)
#define parent            (this->hidden->parent)
#define dma_buf            (this->hidden->dma_buf)
#define dma_len            (this->hidden->dma_len)
#define num_buffers        (this->hidden->num_buffers)
#define frame_ticks        (this->hidden->frame_ticks)
#define next_frame        (this->hidden->next_frame)

#endif /* SDL_dspaudio_h_ */
