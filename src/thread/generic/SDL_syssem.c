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

/* An implementation of semaphores using mutexes and condition variables */

#include "SDL_timer.h"
#include "SDL_thread.h"
#include "SDL_systhread_c.h"


#if SDL_THREADS_DISABLED

SDL_sem *SDL_CreateSemaphore(Uint32 initial_value)
{
	SDL_SetError("SDL not configured with thread support");
	return (SDL_sem *)0;
}

void SDL_DestroySemaphore(SDL_sem *sem)
{
	return;
}

int SDL_SemTryWait(SDL_sem *sem)
{
	SDL_SetError("SDL not configured with thread support");
	return -1;
}

int SDL_SemWaitTimeout(SDL_sem *sem, Uint32 timeout)
{
	SDL_SetError("SDL not configured with thread support");
	return -1;
}

int SDL_SemWait(SDL_sem *sem)
{
	SDL_SetError("SDL not configured with thread support");
	return -1;
}

Uint32 SDL_SemValue(SDL_sem *sem)
{
	return 0;
}

int SDL_SemPost(SDL_sem *sem)
{
	SDL_SetError("SDL not configured with thread support");
	return -1;
}

#else

struct SDL_semaphore {
	Uint32 count;
	Uint32 waiters_count;
	SDL_mutex *count_lock;
	SDL_cond *count_nonzero;
};

SDL_sem *SDL_CreateSemaphore(Uint32 initial_value) {
	SDL_sem *sem;

	sem = (SDL_sem *) SDL_malloc(sizeof(*sem));
	if(!sem) {
		SDL_OutOfMemory();
		return NULL;
	}
	sem->count = initial_value;
	sem->waiters_count = 0;

	sem->count_lock = SDL_CreateMutex();
	sem->count_nonzero = SDL_CreateCond();
	if(!sem->count_lock || !sem->count_nonzero) {
		SDL_DestroySemaphore(sem);
		return NULL;
	}

	return sem;
}

/* WARNING:
   You cannot call this function when another thread is using the semaphore.
*/
void SDL_DestroySemaphore(SDL_sem *sem) {
	if(sem) {
		sem->count = 0xFFFFFFFF;
		while (sem->waiters_count > 0) {
			SDL_CondSignal(sem->count_nonzero);
			SDL_Delay(10);
		}
		SDL_DestroyCond(sem->count_nonzero);
		if(sem->count_lock) {
			SDL_mutexP(sem->count_lock);
			SDL_mutexV(sem->count_lock);
			SDL_DestroyMutex(sem->count_lock);
		}
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
	SDL_LockMutex(sem->count_lock);
	if(sem->count > 0) {
		--sem->count;
		retval = 0;
	}
	SDL_UnlockMutex(sem->count_lock);

	return retval;
}

int SDL_SemWaitTimeout(SDL_sem *sem, Uint32 timeout) {
	int retval;

	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	/* A timeout of 0 is an easy case */
	if(timeout == 0) {
		return SDL_SemTryWait(sem);
	}

	SDL_LockMutex(sem->count_lock);
	++sem->waiters_count;
	retval = 0;
	while ((sem->count == 0) && (retval != SDL_MUTEX_TIMEDOUT)) {
		retval = SDL_CondWaitTimeout(sem->count_nonzero, sem->count_lock, timeout);
	}
	--sem->waiters_count;
	if(retval == 0) {
		--sem->count;
	}
	SDL_UnlockMutex(sem->count_lock);

	return retval;
}

int SDL_SemWait(SDL_sem *sem) {
	return SDL_SemWaitTimeout(sem, SDL_MUTEX_MAXWAIT);
}

Uint32 SDL_SemValue(SDL_sem *sem) {
	Uint32 value;

	value = 0;
	if(sem) {
		SDL_LockMutex(sem->count_lock);
		value = sem->count;
		SDL_UnlockMutex(sem->count_lock);
	}
	return value;
}

int SDL_SemPost(SDL_sem *sem) {
	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	SDL_LockMutex(sem->count_lock);
	if(sem->waiters_count > 0) {
		SDL_CondSignal(sem->count_nonzero);
	}
	++sem->count;
	SDL_UnlockMutex(sem->count_lock);

	return 0;
}

#endif /* SDL_THREADS_DISABLED */
