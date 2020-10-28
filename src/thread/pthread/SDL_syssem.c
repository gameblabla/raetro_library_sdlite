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

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/time.h>

#include "SDL_thread.h"
#include "SDL_timer.h"

/* Wrapper around POSIX 1003.1b semaphores */

#ifdef __MACOSX__
/* Mac OS X doesn't support sem_getvalue() as of version 10.4 */
#include "../generic/SDL_syssem.c"
#else

struct SDL_semaphore {
	sem_t sem;
};

/* Create a semaphore, initialized with value */
SDL_sem *SDL_CreateSemaphore(Uint32 initial_value) {
	SDL_sem *sem = (SDL_sem *) SDL_malloc(sizeof(SDL_sem));
	if(sem) {
		if(sem_init(&sem->sem, 0, initial_value) < 0) {
			SDL_SetError("sem_init() failed");
			SDL_free(sem);
			sem = NULL;
		}
	} else {
		SDL_OutOfMemory();
	}
	return sem;
}

void SDL_DestroySemaphore(SDL_sem *sem) {
	if(sem) {
		sem_destroy(&sem->sem);
		SDL_free(sem);
	}
}

int SDL_SemTryWait(SDL_sem *sem) {
	int retval;

	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}
	retval = SDL_MUTEX_TIMEDOUT;
	if(sem_trywait(&sem->sem) == 0) {
		retval = 0;
	}
	return retval;
}

int SDL_SemWait(SDL_sem *sem) {
	int retval;

	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	while (((retval = sem_wait(&sem->sem)) == -1) && (errno == EINTR)) {
	}
	if(retval < 0) {
		SDL_SetError("sem_wait() failed");
	}
	return retval;
}

int SDL_SemWaitTimeout(SDL_sem *sem, Uint32 timeout) {
	int retval;
#ifdef HAVE_SEM_TIMEDWAIT
	struct timeval now;
	struct timespec ts_timeout;
#else
	Uint32 end;
#endif

	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	/* Try the easy cases first */
	if(timeout == 0) {
		return SDL_SemTryWait(sem);
	}
	if(timeout == SDL_MUTEX_MAXWAIT) {
		return SDL_SemWait(sem);
	}

#ifdef HAVE_SEM_TIMEDWAIT
	/* Setup the timeout. sem_timedwait doesn't wait for
	 * a lapse of time, but until we reach a certain time.
	 * This time is now plus the timeout.
	 */
	gettimeofday(&now, NULL);

	/* Add our timeout to current time */
	now.tv_usec += (timeout % 1000) * 1000;
	now.tv_sec += timeout / 1000;

	/* Wrap the second if needed */
	if(now.tv_usec >= 1000000) {
		now.tv_usec -= 1000000;
		now.tv_sec++;
	}

	/* Convert to timespec */
	ts_timeout.tv_sec = now.tv_sec;
	ts_timeout.tv_nsec = now.tv_usec * 1000;

	/* Wait. */
	do {
		retval = sem_timedwait(&sem->sem, &ts_timeout);
	} while (retval == -1 && errno == EINTR);

	if(retval == -1) {
		if(errno == ETIMEDOUT) {
			retval = SDL_MUTEX_TIMEDOUT;
		} else {
			SDL_SetError(strerror(errno));
		}
	}
#else
	end = SDL_GetTicks() + timeout;
	while ((retval = SDL_SemTryWait(sem)) == SDL_MUTEX_TIMEDOUT) {
		if ((SDL_GetTicks() - end) >= 0) {
			break;
		}
		SDL_Delay(0);
	}
#endif /* HAVE_SEM_TIMEDWAIT */

	return retval;
}

Uint32 SDL_SemValue(SDL_sem *sem) {
	int ret = 0;
	if(sem) {
		sem_getvalue(&sem->sem, &ret);
		if(ret < 0) {
			ret = 0;
		}
	}
	return (Uint32) ret;
}

int SDL_SemPost(SDL_sem *sem) {
	int retval;

	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	retval = sem_post(&sem->sem);
	if(retval < 0) {
		SDL_SetError("sem_post() failed");
	}
	return retval;
}

#endif /* __MACOSX__ */
