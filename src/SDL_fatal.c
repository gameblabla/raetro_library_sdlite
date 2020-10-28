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

/* General fatal signal handling code for SDL */

#ifdef HAVE_SIGNAL_H

#include <signal.h>

#include "SDL.h"
#include "SDL_fatal.h"

/* This installs some signal handlers for the more common fatal signals,
   so that if the programmer is lazy, the app doesn't die so horribly if
   the program crashes.
*/

static void SDL_Parachute(int sig) {
	signal(sig, SIG_DFL);
	SDL_Quit();
	raise(sig);
}

static int SDL_fatal_signals[] = {
	SIGSEGV,
#ifdef SIGBUS
	SIGBUS,
#endif
#ifdef SIGFPE
	SIGFPE,
#endif
#ifdef SIGQUIT
	SIGQUIT,
#endif
	0
};

void SDL_InstallParachute(void) {
	/* Set a handler for any fatal signal not already handled */
	int i;
#ifdef HAVE_SIGACTION
	struct sigaction action;

	for (i = 0; SDL_fatal_signals[i]; ++i) {
		sigaction(SDL_fatal_signals[i], NULL, &action);
		if(action.sa_handler == SIG_DFL) {
			action.sa_handler = SDL_Parachute;
			sigaction(SDL_fatal_signals[i], &action, NULL);
		}
	}
#ifdef SIGALRM
	/* Set SIGALRM to be ignored -- necessary on Solaris */
	sigaction(SIGALRM, NULL, &action);
	if(action.sa_handler == SIG_DFL) {
		action.sa_handler = SIG_IGN;
		sigaction(SIGALRM, &action, NULL);
	}
#endif
#else
	void (*ohandler)(int);

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SDL_Parachute);
		if ( ohandler != SIG_DFL ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
#endif /* HAVE_SIGACTION */
	return;
}

void SDL_UninstallParachute(void) {
	/* Remove a handler for any fatal signal handled */
	int i;
#ifdef HAVE_SIGACTION
	struct sigaction action;

	for (i = 0; SDL_fatal_signals[i]; ++i) {
		sigaction(SDL_fatal_signals[i], NULL, &action);
		if(action.sa_handler == SDL_Parachute) {
			action.sa_handler = SIG_DFL;
			sigaction(SDL_fatal_signals[i], &action, NULL);
		}
	}
#else
	void (*ohandler)(int);

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SIG_DFL);
		if ( ohandler != SDL_Parachute ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
#endif /* HAVE_SIGACTION */
}

#else

/* No signals on this platform, nothing to do.. */

void SDL_InstallParachute(void) {
	return;
}

void SDL_UninstallParachute(void) {
	return;
}

#endif /* HAVE_SIGNAL_H */
