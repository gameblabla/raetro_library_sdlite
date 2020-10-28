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

/* This file defines a structure that carries language-independent
   error messages
*/

#ifndef SDL_error_c_h_
#define SDL_error_c_h_

#define ERR_MAX_STRLEN    128
#define ERR_MAX_ARGS    5

typedef struct SDL_error {
	/* This is a numeric value corresponding to the current error */
	int error;

	/* This is a key used to index into a language hashtable containing
	   internationalized versions of the SDL error messages.  If the key
	   is not in the hashtable, or no hashtable is available, the key is
	   used directly as an error message format string.
	*/
	char key[ERR_MAX_STRLEN];

	/* These are the arguments for the error functions */
	int argc;
	union {
		void *value_ptr;
#if 0    /* What is a character anyway?  (UNICODE issues) */
		unsigned char value_c;
#endif
		int value_i;
		long value_l;
		double value_f;
		char buf[ERR_MAX_STRLEN];
	} args[ERR_MAX_ARGS];
} SDL_error;

#endif /* SDL_error_c_h_ */
