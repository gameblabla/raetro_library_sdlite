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

#ifndef SDL_main_h_
#define SDL_main_h_

#include "SDL_stdinc.h"

/** @file SDL_main.h
 *  Redefine main() on Win32 and MacOS so that it is called by winmain.c
 */

#if defined(__WIN32__) || \
    (defined(__MWERKS__) && !defined(__BEOS__)) || \
    defined(__MACOS__) || defined(__MACOSX__) || \
    defined(__SYMBIAN32__) || defined(QWS)

#ifdef __cplusplus
#define C_LINKAGE	"C"
#else
#define C_LINKAGE
#endif /* __cplusplus */

/** The application's main() function must be called with C linkage,
 *  and should be declared like this:
 *      @code
 *      #ifdef __cplusplus
 *      extern "C"
 *      #endif
 *	int main(int argc, char *argv[])
 *	{
 *	}
 *      @endcode
 */
#define main	SDL_main

/** The prototype for the application's main() function */
extern C_LINKAGE int SDL_main(int argc, char *argv[]);


/** @name From the SDL library code -- needed for registering the app on Win32 */
/*@{*/
#ifdef __WIN32__

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

/** This should be called from your WinMain() function, if any */
extern DECLSPEC void SDLCALL SDL_SetModuleHandle(void *hInst);
/** This can also be called, but is no longer necessary */
extern DECLSPEC int SDLCALL SDL_RegisterApp(char *name, Uint32 style, void *hInst);
/** This can also be called, but is no longer necessary (SDL_Quit calls it) */
extern DECLSPEC void SDLCALL SDL_UnregisterApp(void);
#ifdef __cplusplus
}
#endif
#include "close_code.h"
#endif
/*@}*/

/** @name From the SDL library code -- needed for registering QuickDraw on MacOS */
/*@{*/
#if defined(__MACOS__)

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

/** Forward declaration so we don't need to include QuickDraw.h */
struct QDGlobals;

/** This should be called from your main() function, if any */
extern DECLSPEC void SDLCALL SDL_InitQuickDraw(struct QDGlobals *the_qd);

#ifdef __cplusplus
}
#endif
#include "close_code.h"
#endif
/*@}*/

#endif /* Need to redefine main()? */

#endif /* SDL_main_h_ */
