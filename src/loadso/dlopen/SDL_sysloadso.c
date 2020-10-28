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

#ifdef SDL_LOADSO_DLOPEN

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* System dependent library loading routines                           */

#include <stdio.h>
#include <dlfcn.h>

#include "SDL_loadso.h"

void *SDL_LoadObject(const char *sofile) {
	void *handle = dlopen(sofile, RTLD_NOW);
	const char *loaderror = (char *) dlerror();
	if(handle == NULL) {
		SDL_SetError("Failed loading %s: %s", sofile, loaderror);
	}
	return (handle);
}

void *SDL_LoadFunction(void *handle, const char *name) {
	void *symbol = dlsym(handle, name);
	if(symbol == NULL) {
		/* append an underscore for platforms that need that. */
		size_t len = 1 + SDL_strlen(name) + 1;
		char *_name = SDL_stack_alloc(char, len);
		_name[0] = '_';
		SDL_strlcpy(&_name[1], name, len);
		symbol = dlsym(handle, _name);
		SDL_stack_free(_name);
		if(symbol == NULL) {
			SDL_SetError("Failed loading %s: %s", name, (const char *) dlerror());
		}
	}
	return (symbol);
}

void SDL_UnloadObject(void *handle) {
	if(handle != NULL) {
		dlclose(handle);
	}
}

#endif /* SDL_LOADSO_DLOPEN */
