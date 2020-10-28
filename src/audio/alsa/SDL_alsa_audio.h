/*******************************************************************************
 * Library       : SDLite 1.2.x
 * Purpose       : Low-level access to a framebuffer, audio output and HID.
 * Module        : Core
 * Project       : Redux for Embedded System
 * Description   : Stripped-down and optimized libraries for RISC processors
 * License       : GNU General Public License v3.0
 *******************************************************************************
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

#ifndef _ALSA_PCM_audio_h
#define _ALSA_PCM_audio_h

#include <alsa/asoundlib.h>

#include "../SDL_sysaudio.h"

/* Hidden "this" pointer for the video functions */
#define _THIS SDL_AudioDevice *this

struct SDL_PrivateAudioData {
	/* The audio device handle */
	snd_pcm_t *pcm_handle;

	/* Raw mixing buffer */
	Uint8 *mixbuf;
	int mixlen;
};

/* Old variable names */
#define pcm_handle (this->hidden->pcm_handle)
#define mixbuf     (this->hidden->mixbuf)
#define mixlen     (this->hidden->mixlen)

#endif /* _ALSA_PCM_audio_h */
