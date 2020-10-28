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

#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "SDL_thread.h"
#include "SDL_sysmutex_c.h"

struct SDL_cond {
	pthread_cond_t cond;
};

/* Create a condition variable */
SDL_cond *SDL_CreateCond(void) {
	SDL_cond *cond;

	cond = (SDL_cond *) SDL_malloc(sizeof(SDL_cond));
	if(cond) {
		if(pthread_cond_init(&cond->cond, NULL) < 0) {
			SDL_SetError("pthread_cond_init() failed");
			SDL_free(cond);
			cond = NULL;
		}
	}
	return (cond);
}

/* Destroy a condition variable */
void SDL_DestroyCond(SDL_cond *cond) {
	if(cond) {
		pthread_cond_destroy(&cond->cond);
		SDL_free(cond);
	}
}

/* Restart one of the threads that are waiting on the condition variable */
int SDL_CondSignal(SDL_cond *cond) {
	int retval;

	if(!cond) {
		SDL_SetError("Passed a NULL condition variable");
		return -1;
	}

	retval = 0;
	if(pthread_cond_signal(&cond->cond) != 0) {
		SDL_SetError("pthread_cond_signal() failed");
		retval = -1;
	}
	return retval;
}

/* Restart all threads that are waiting on the condition variable */
int SDL_CondBroadcast(SDL_cond *cond) {
	int retval;

	if(!cond) {
		SDL_SetError("Passed a NULL condition variable");
		return -1;
	}

	retval = 0;
	if(pthread_cond_broadcast(&cond->cond) != 0) {
		SDL_SetError("pthread_cond_broadcast() failed");
		retval = -1;
	}
	return retval;
}

int SDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, Uint32 ms) {
	int retval;
	struct timeval delta;
	struct timespec abstime;

	if(!cond) {
		SDL_SetError("Passed a NULL condition variable");
		return -1;
	}

	gettimeofday(&delta, NULL);

	abstime.tv_sec = delta.tv_sec + (ms / 1000);
	abstime.tv_nsec = (delta.tv_usec + (ms % 1000) * 1000) * 1000;
	if(abstime.tv_nsec > 1000000000) {
		abstime.tv_sec += 1;
		abstime.tv_nsec -= 1000000000;
	}

tryagain:
	retval = pthread_cond_timedwait(&cond->cond, &mutex->id, &abstime);
	switch (retval) {
		case EINTR:
			goto tryagain;
			break;
		case ETIMEDOUT:
			retval = SDL_MUTEX_TIMEDOUT;
			break;
		case 0:
			break;
		default:
			SDL_SetError("pthread_cond_timedwait() failed");
			retval = -1;
			break;
	}
	return retval;
}

/* Wait on the condition variable, unlocking the provided mutex.
   The mutex must be locked before entering this function!
 */
int SDL_CondWait(SDL_cond *cond, SDL_mutex *mutex) {
	int retval;

	if(!cond) {
		SDL_SetError("Passed a NULL condition variable");
		return -1;
	}

	retval = 0;
	if(pthread_cond_wait(&cond->cond, &mutex->id) != 0) {
		SDL_SetError("pthread_cond_wait() failed");
		retval = -1;
	}
	return retval;
}
