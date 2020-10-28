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

/** @file SDL_loadso.h
 *  System dependent library loading routines
 */

/** @file SDL_loadso.h
 *  Some things to keep in mind:                                        
 *  - These functions only work on C function names.  Other languages may
 *    have name mangling and intrinsic language support that varies from
 *    compiler to compiler.
 *  - Make sure you declare your function pointers with the same calling
 *    convention as the actual library function.  Your code will crash
 *    mysteriously if you do not do this.
 *  - Avoid namespace collisions.  If you load a symbol from the library,
 *    it is not defined whether or not it goes into the global symbol
 *    namespace for the application.  If it does and it conflicts with
 *    symbols in your code or other shared libraries, you will not get
 *    the results you expect. :)
 */


#ifndef SDL_loadso_h_
#define SDL_loadso_h_

#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function dynamically loads a shared object and returns a pointer
 * to the object handle (or NULL if there was an error).
 * The 'sofile' parameter is a system dependent name of the object file.
 */
extern DECLSPEC void *SDLCALL SDL_LoadObject(const char *sofile);

/**
 * Given an object handle, this function looks up the address of the
 * named function in the shared object and returns it.  This address
 * is no longer valid after calling SDL_UnloadObject().
 */
extern DECLSPEC void *SDLCALL SDL_LoadFunction(void *handle, const char *name);

/** Unload a shared object from memory */
extern DECLSPEC void SDLCALL SDL_UnloadObject(void *handle);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_loadso_h_ */
