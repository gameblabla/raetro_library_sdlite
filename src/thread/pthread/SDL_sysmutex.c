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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <pthread.h>

#include "SDL_thread.h"

#if !SDL_THREAD_PTHREAD_RECURSIVE_MUTEX && \
    !SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP
#define FAKE_RECURSIVE_MUTEX
#endif

struct SDL_mutex {
	pthread_mutex_t id;
#if FAKE_RECURSIVE_MUTEX
	int recursive;
	pthread_t owner;
#endif
};

SDL_mutex *SDL_CreateMutex(void) {
	SDL_mutex *mutex;
	pthread_mutexattr_t attr;

	/* Allocate the structure */
	mutex = (SDL_mutex *) SDL_calloc(1, sizeof(*mutex));
	if(mutex) {
		pthread_mutexattr_init(&attr);
#if SDL_THREAD_PTHREAD_RECURSIVE_MUTEX
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#elif SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP
		pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#else
		/* No extra attributes necessary */
#endif
		if(pthread_mutex_init(&mutex->id, &attr) != 0) {
			SDL_SetError("pthread_mutex_init() failed");
			SDL_free(mutex);
			mutex = NULL;
		}
	} else {
		SDL_OutOfMemory();
	}
	return (mutex);
}

void SDL_DestroyMutex(SDL_mutex *mutex) {
	if(mutex) {
		pthread_mutex_destroy(&mutex->id);
		SDL_free(mutex);
	}
}

/* Lock the mutex */
int SDL_mutexP(SDL_mutex *mutex) {
	int retval;
#if FAKE_RECURSIVE_MUTEX
	pthread_t this_thread;
#endif

	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	retval = 0;
#if FAKE_RECURSIVE_MUTEX
	this_thread = pthread_self();
	if ( mutex->owner == this_thread ) {
		++mutex->recursive;
	} else {
		/* The order of operations is important.
		   We set the locking thread id after we obtain the lock
		   so unlocks from other threads will fail.
		*/
		if ( pthread_mutex_lock(&mutex->id) == 0 ) {
			mutex->owner = this_thread;
			mutex->recursive = 0;
		} else {
			SDL_SetError("pthread_mutex_lock() failed");
			retval = -1;
		}
	}
#else
	if(pthread_mutex_lock(&mutex->id) < 0) {
		SDL_SetError("pthread_mutex_lock() failed");
		retval = -1;
	}
#endif
	return retval;
}

int SDL_mutexV(SDL_mutex *mutex) {
	int retval;

	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	retval = 0;
#if FAKE_RECURSIVE_MUTEX
	/* We can only unlock the mutex if we own it */
	if ( pthread_self() == mutex->owner ) {
		if ( mutex->recursive ) {
			--mutex->recursive;
		} else {
			/* The order of operations is important.
			   First reset the owner so another thread doesn't lock
			   the mutex and set the ownership before we reset it,
			   then release the lock semaphore.
			 */
			mutex->owner = 0;
			pthread_mutex_unlock(&mutex->id);
		}
	} else {
		SDL_SetError("mutex not owned by this thread");
		retval = -1;
	}

#else
	if(pthread_mutex_unlock(&mutex->id) < 0) {
		SDL_SetError("pthread_mutex_unlock() failed");
		retval = -1;
	}
#endif /* FAKE_RECURSIVE_MUTEX */

	return retval;
}
