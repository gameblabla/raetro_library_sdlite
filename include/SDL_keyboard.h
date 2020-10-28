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

/** @file SDL_keyboard.h
 *  Include file for SDL keyboard event handling
 */

#ifndef SDL_keyboard_h_
#define SDL_keyboard_h_

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_keysym.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/** Keysym structure
 *
 *  - The scancode is hardware dependent, and should not be used by general
 *    applications.  If no hardware scancode is available, it will be 0.
 *
 *  - The 'unicode' translated character is only available when character
 *    translation is enabled by the SDL_EnableUNICODE() API.  If non-zero,
 *    this is a UNICODE character corresponding to the keypress.  If the
 *    high 9 bits of the character are 0, then this maps to the equivalent
 *    ASCII character:
 *      @code
 *	char ch;
 *	if ( (keysym.unicode & 0xFF80) == 0 ) {
 *		ch = keysym.unicode & 0x7F;
 *	} else {
 *		An international character..
 *	}
 *      @endcode
 */
typedef struct SDL_keysym {
	Uint8 scancode;            /**< hardware specific scancode */
	SDLKey sym;            /**< SDL virtual keysym */
	SDLMod mod;            /**< current key modifiers */
	Uint16 unicode;            /**< translated character */
} SDL_keysym;

/** This is the mask which refers to all hotkey bindings */
#define SDL_ALL_HOTKEYS        0xFFFFFFFF

/* Function prototypes */
/**
 * Enable/Disable UNICODE translation of keyboard input.
 *
 * This translation has some overhead, so translation defaults off.
 *
 * @param[in] enable
 * If 'enable' is 1, translation is enabled.
 * If 'enable' is 0, translation is disabled.
 * If 'enable' is -1, the translation state is not changed.
 *
 * @return It returns the previous state of keyboard translation.
 */
extern DECLSPEC int SDLCALL SDL_EnableUNICODE(int enable);

#define SDL_DEFAULT_REPEAT_DELAY    500
#define SDL_DEFAULT_REPEAT_INTERVAL    30

/**
 * Enable/Disable keyboard repeat.  Keyboard repeat defaults to off.
 *
 *  @param[in] delay
 *  'delay' is the initial delay in ms between the time when a key is
 *  pressed, and keyboard repeat begins.
 *
 *  @param[in] interval
 *  'interval' is the time in ms between keyboard repeat events.
 *
 *  If 'delay' is set to 0, keyboard repeat is disabled.
 */
extern DECLSPEC int SDLCALL SDL_EnableKeyRepeat(int delay, int interval);

extern DECLSPEC void SDLCALL SDL_GetKeyRepeat(int *delay, int *interval);

/**
 * Get a snapshot of the current state of the keyboard.
 * Returns an array of keystates, indexed by the SDLK_* syms.
 * Usage:
 *	@code
 * 	Uint8 *keystate = SDL_GetKeyState(NULL);
 *	if ( keystate[SDLK_RETURN] ) //... \<RETURN> is pressed.
 *	@endcode
 */
extern DECLSPEC Uint8 *SDLCALL SDL_GetKeyState(int *numkeys);

/**
 * Get the current key modifier state
 */
extern DECLSPEC SDLMod SDLCALL SDL_GetModState(void);

/**
 * Set the current key modifier state.
 * This does not change the keyboard state, only the key modifier flags.
 */
extern DECLSPEC void SDLCALL SDL_SetModState(SDLMod modstate);

/**
 * Get the name of an SDL virtual keysym
 */
extern DECLSPEC char *SDLCALL SDL_GetKeyName(SDLKey key);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_keyboard_h_ */