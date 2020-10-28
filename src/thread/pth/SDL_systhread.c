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
 *	GNU pth threads
 *
 *	Patrice Mandin
 */

#include <pth.h>
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

static void *RunThread(void *data) {
	SDL_RunThread(data);
	pth_exit((void *) 0);
	return ((void *) 0);        /* Prevent compiler warning */
}

int SDL_SYS_CreateThread(SDL_Thread *thread, void *args) {
	pth_attr_t type;

	/* Create a new attribute */
	type = pth_attr_new();
	if(type == NULL) {
		SDL_SetError("Couldn't initialize pth attributes");
		return (-1);
	}
	pth_attr_set(type, PTH_ATTR_JOINABLE, TRUE);

	/* Create the thread and go! */
	thread->handle = pth_spawn(type, RunThread, args);
	if(thread->handle == NULL) {
		SDL_SetError("Not enough resources to create thread");
		return (-1);
	}
	return (0);
}

void SDL_SYS_SetupThread(void) {
	int i;
	sigset_t mask;
	int oldstate;

	/* Mask asynchronous signals for this thread */
	sigemptyset(&mask);
	for (i = 0; sig_list[i]; ++i) {
		sigaddset(&mask, sig_list[i]);
	}
	pth_sigmask(SIG_BLOCK, &mask, 0);

	/* Allow ourselves to be asynchronously cancelled */
	pth_cancel_state(PTH_CANCEL_ASYNCHRONOUS, &oldstate);
}

/* WARNING:  This may not work for systems with 64-bit pid_t */
Uint32 SDL_ThreadID(void) {
	return ((Uint32) pth_self());
}

void SDL_SYS_WaitThread(SDL_Thread *thread) {
	pth_join(thread->handle, NULL);
}

void SDL_SYS_KillThread(SDL_Thread *thread) {
	pth_cancel(thread->handle);
	pth_join(thread->handle, NULL);
}
