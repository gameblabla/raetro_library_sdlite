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

/* An implementation of mutexes using semaphores */

#include "SDL_thread.h"
#include "SDL_systhread_c.h"


struct SDL_mutex {
	int recursive;
	Uint32 owner;
	SDL_sem *sem;
};

/* Create a mutex */
SDL_mutex *SDL_CreateMutex(void) {
	SDL_mutex *mutex;

	/* Allocate mutex memory */
	mutex = (SDL_mutex *) SDL_malloc(sizeof(*mutex));
	if(mutex) {
		/* Create the mutex semaphore, with initial value 1 */
		mutex->sem = SDL_CreateSemaphore(1);
		mutex->recursive = 0;
		mutex->owner = 0;
		if(!mutex->sem) {
			SDL_free(mutex);
			mutex = NULL;
		}
	} else {
		SDL_OutOfMemory();
	}
	return mutex;
}

/* Free the mutex */
void SDL_DestroyMutex(SDL_mutex *mutex) {
	if(mutex) {
		if(mutex->sem) {
			SDL_DestroySemaphore(mutex->sem);
		}
		SDL_free(mutex);
	}
}

/* Lock the semaphore */
int SDL_mutexP(SDL_mutex *mutex) {
#if SDL_THREADS_DISABLED
	return 0;
#else
	Uint32 this_thread;

	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	this_thread = SDL_ThreadID();
	if(mutex->owner == this_thread) {
		++mutex->recursive;
	} else {
		/* The order of operations is important.
		   We set the locking thread id after we obtain the lock
		   so unlocks from other threads will fail.
		*/
		SDL_SemWait(mutex->sem);
		mutex->owner = this_thread;
		mutex->recursive = 0;
	}

	return 0;
#endif /* SDL_THREADS_DISABLED */
}

/* Unlock the mutex */
int SDL_mutexV(SDL_mutex *mutex) {
#if SDL_THREADS_DISABLED
	return 0;
#else
	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	/* If we don't own the mutex, we can't unlock it */
	if(SDL_ThreadID() != mutex->owner) {
		SDL_SetError("mutex not owned by this thread");
		return -1;
	}

	if(mutex->recursive) {
		--mutex->recursive;
	} else {
		/* The order of operations is important.
		   First reset the owner so another thread doesn't lock
		   the mutex and set the ownership before we reset it,
		   then release the lock semaphore.
		 */
		mutex->owner = 0;
		SDL_SemPost(mutex->sem);
	}
	return 0;
#endif /* SDL_THREADS_DISABLED */
}
