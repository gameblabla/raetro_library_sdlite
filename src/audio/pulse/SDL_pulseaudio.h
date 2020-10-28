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

#ifndef SDL_pulseaudio_h_
#define SDL_pulseaudio_h_

#include "../SDL_sysaudio.h"

/* Hidden "this" pointer for the video functions */
#define _THIS    SDL_AudioDevice *this

struct SDL_PrivateAudioData {
	pa_mainloop *mainloop;
	pa_mainloop_api *mainloop_api;
	pa_context *context;
	pa_stream *stream;

	char *caption;

	/* Raw mixing buffer */
	Uint8 *mixbuf;
	int mixlen;
};

#if (PA_API_VERSION < 12)
/** Return non-zero if the passed state is one of the connected states */
static inline int PA_CONTEXT_IS_GOOD(pa_context_state_t x) {
	return x == PA_CONTEXT_CONNECTING || x == PA_CONTEXT_AUTHORIZING || x == PA_CONTEXT_SETTING_NAME || x == PA_CONTEXT_READY;
}

/** Return non-zero if the passed state is one of the connected states */
static inline int PA_STREAM_IS_GOOD(pa_stream_state_t x) {
	return x == PA_STREAM_CREATING || x == PA_STREAM_READY;
}

#endif /* pulseaudio <= 0.9.10 */

/* Old variable names */
#define mainloop     (this->hidden->mainloop)
#define mainloop_api (this->hidden->mainloop_api)
#define context      (this->hidden->context)
#define stream       (this->hidden->stream)
#define mixbuf       (this->hidden->mixbuf)
#define mixlen       (this->hidden->mixlen)

#endif /* SDL_pulseaudio_h_ */
