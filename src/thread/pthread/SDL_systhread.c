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

#include <pthread.h>
#include <signal.h>

#include "SDL_thread.h"
#include "../SDL_thread_c.h"
#include "../SDL_systhread.h"

/* List of signals to mask in the subthreads */
static int sig_list[] = {
	SIGHUP,
	SIGINT,
	SIGQUIT,
	SIGPIPE,
	SIGALRM,
	SIGTERM,
	SIGCHLD,
	SIGWINCH,
	SIGVTALRM,
	SIGPROF,
	0
};

#ifdef __RISCOS__
/* RISC OS needs to know the main thread for
 * it's timer and event processing. */
int riscos_using_threads = 0;
Uint32 riscos_main_thread = 0; /* Thread running events */
#endif


static void *RunThread(void *data) {
	SDL_RunThread(data);
	pthread_exit((void *) 0);
	return ((void *) 0);        /* Prevent compiler warning */
}

int SDL_SYS_CreateThread(SDL_Thread *thread, void *args) {
	pthread_attr_t type;

	/* Set the thread attributes */
	if(pthread_attr_init(&type) != 0) {
		SDL_SetError("Couldn't initialize pthread attributes");
		return (-1);
	}
	pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);

	/* Create the thread and go! */
	if(pthread_create(&thread->handle, &type, RunThread, args) != 0) {
		SDL_SetError("Not enough resources to create thread");
		return (-1);
	}

#ifdef __RISCOS__
	if (riscos_using_threads == 0) {
		riscos_using_threads = 1;
		riscos_main_thread = SDL_ThreadID();
	}
#endif

	return (0);
}

void SDL_SYS_SetupThread(void) {
	int i;
	sigset_t mask;

	/* Mask asynchronous signals for this thread */
	sigemptyset(&mask);
	for (i = 0; sig_list[i]; ++i) {
		sigaddset(&mask, sig_list[i]);
	}
	pthread_sigmask(SIG_BLOCK, &mask, 0);

#ifdef PTHREAD_CANCEL_ASYNCHRONOUS
	/* Allow ourselves to be asynchronously cancelled */
	{
		int oldstate;
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldstate);
	}
#endif
}

/* WARNING:  This may not work for systems with 64-bit pid_t */
Uint32 SDL_ThreadID(void) {
	return ((Uint32) ((size_t) pthread_self()));
}

void SDL_SYS_WaitThread(SDL_Thread *thread) {
	pthread_join(thread->handle, 0);
}

void SDL_SYS_KillThread(SDL_Thread *thread) {
#ifdef PTHREAD_CANCEL_ASYNCHRONOUS
	pthread_cancel(thread->handle);
#else
#ifdef __FREEBSD__
#warning For some reason, this doesnt actually kill a thread - FreeBSD 3.2
#endif
	pthread_kill(thread->handle, SIGKILL);
#endif
}
