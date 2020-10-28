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

/*
 *	GNU pth mutexes
 *
 *	Patrice Mandin
 */

#include <pth.h>

#include "SDL_mutex.h"
#include "SDL_sysmutex_c.h"

/* Create a mutex */
SDL_mutex *SDL_CreateMutex(void) {
	SDL_mutex *mutex;

	/* Allocate mutex memory */
	mutex = (SDL_mutex *) SDL_malloc(sizeof(*mutex));
	if(mutex) {
		/* Create the mutex, with initial value signaled */
		if(!pth_mutex_init(&(mutex->mutexpth_p))) {
			SDL_SetError("Couldn't create mutex");
			SDL_free(mutex);
			mutex = NULL;
		}
	} else {
		SDL_OutOfMemory();
	}
	return (mutex);
}

/* Free the mutex */
void SDL_DestroyMutex(SDL_mutex *mutex) {
	if(mutex) {
		SDL_free(mutex);
	}
}

/* Lock the mutex */
int SDL_mutexP(SDL_mutex *mutex) {
	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	pth_mutex_acquire(&(mutex->mutexpth_p), FALSE, NULL);

	return (0);
}

/* Unlock the mutex */
int SDL_mutexV(SDL_mutex *mutex) {
	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	pth_mutex_release(&(mutex->mutexpth_p));

	return (0);
}
