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

/* Useful variables and functions from SDL_cursor.c */
#include "SDL_mouse.h"

extern int SDL_CursorInit(Uint32 flags);

extern void SDL_CursorPaletteChanged(void);

extern void SDL_DrawCursor(SDL_Surface *screen);

extern void SDL_DrawCursorNoLock(SDL_Surface *screen);

extern void SDL_EraseCursor(SDL_Surface *screen);

extern void SDL_EraseCursorNoLock(SDL_Surface *screen);

extern void SDL_UpdateCursor(SDL_Surface *screen);

extern void SDL_ResetCursor(void);

extern void SDL_MoveCursor(int x, int y);

extern void SDL_CursorQuit(void);

#define INLINE_MOUSELOCK
#ifdef INLINE_MOUSELOCK
/* Inline (macro) versions of the mouse lock functions */
#include "SDL_mutex.h"

extern SDL_mutex *SDL_cursorlock;

#define SDL_LockCursor()                \
    do {                                \
        if ( SDL_cursorlock ) {         \
            SDL_mutexP(SDL_cursorlock); \
        }                               \
    } while ( 0 )
#define SDL_UnlockCursor()              \
    do {                                \
        if ( SDL_cursorlock ) {         \
            SDL_mutexV(SDL_cursorlock); \
        }                               \
    } while ( 0 )
#else
extern void SDL_LockCursor(void);
extern void SDL_UnlockCursor(void);
#endif /* INLINE_MOUSELOCK */

/* Only for low-level mouse cursor drawing */
extern SDL_Cursor *SDL_cursor;

extern void SDL_MouseRect(SDL_Rect *area);

/* State definitions for the SDL cursor */
#define CURSOR_VISIBLE        0x01
#define CURSOR_USINGSW        0x10
#define SHOULD_DRAWCURSOR(X)  (((X)&(CURSOR_VISIBLE|CURSOR_USINGSW)) == (CURSOR_VISIBLE|CURSOR_USINGSW))

extern volatile int SDL_cursorstate;
