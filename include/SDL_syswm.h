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

/** @file SDL_syswm.h
 *  Include file for SDL custom system window manager hooks
 */

#ifndef SDL_syswm_h_
#define SDL_syswm_h_

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_version.h"

#ifndef SDL_PROTOTYPES_ONLY
# if defined(SDL_VIDEO_DRIVER_X11)
#  include <X11/Xlib.h>
#  include <X11/Xatom.h>
# endif
#endif

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/** @file SDL_syswm.h
 *  Your application has access to a special type of event 'SDL_SYSWMEVENT',
 *  which contains window-manager specific information and arrives whenever
 *  an unhandled window event occurs.  This event is ignored by default, but
 *  you can enable it with SDL_EventState()
 */
struct SDL_SysWMinfo;
typedef struct SDL_SysWMinfo SDL_SysWMinfo;

#ifndef SDL_PROTOTYPES_ONLY

/* This is the structure for custom window manager events */
#if defined(SDL_VIDEO_DRIVER_X11)

/** These are the various supported subsystems under UNIX */
typedef enum {
	SDL_SYSWM_X11
} SDL_SYSWM_TYPE;

/** The UNIX custom event structure */
struct SDL_SysWMmsg {
	SDL_version version;
	SDL_SYSWM_TYPE subsystem;
	union {
		XEvent xevent;
	} event;
};

/** The UNIX custom window manager information structure.
 *  When this structure is returned, it holds information about which
 *  low level system it is using, and will be one of SDL_SYSWM_TYPE.
 */
struct SDL_SysWMinfo {
	SDL_version version;
	SDL_SYSWM_TYPE subsystem;
	union {
		struct {
			Display *display;    /**< The X11 display */
			Window window;        /**< The X11 display window */
			/** These locking functions should be called around
					 *  any X11 functions using the display variable,
					 *  but not the gfxdisplay variable.
					 *  They lock the event thread, so should not be
			 *  called around event functions or from event filters.
			 */
			/*@{*/
			void (*lock_func)(void);

			void (*unlock_func)(void);
			/*@}*/

			/** @name Introduced in SDL 1.0.2 */
			/*@{*/
			Window fswindow;    /**< The X11 fullscreen window */
			Window wmwindow;    /**< The X11 managed input window */
			/*@}*/

			/** @name Introduced in SDL 1.2.12 */
			/*@{*/
			Display *gfxdisplay;    /**< The X11 display to which rendering is done */
			/*@}*/
		} x11;
	} info;
};

#elif defined(SDL_VIDEO_DRIVER_NANOX)

/** The generic custom event structure */
struct SDL_SysWMmsg {
	SDL_version version;
	int data;
};

/** The windows custom window manager information structure */
struct SDL_SysWMinfo {
	SDL_version version ;
	GR_WINDOW_ID window ;	/* The display window */
};

#elif defined(SDL_VIDEO_DRIVER_WINDIB) || defined(SDL_VIDEO_DRIVER_DDRAW) || defined(SDL_VIDEO_DRIVER_GAPI)

/** The windows custom event structure */
struct SDL_SysWMmsg {
	SDL_version version;
	HWND hwnd;			/**< The window for the message */
	UINT msg;			/**< The type of message */
	WPARAM wParam;			/**< WORD message parameter */
	LPARAM lParam;			/**< LONG message parameter */
};

/** The windows custom window manager information structure */
struct SDL_SysWMinfo {
	SDL_version version;
	HWND window;			/**< The Win32 display window */
	HGLRC hglrc;			/**< The OpenGL context, if any */
};

#elif defined(SDL_VIDEO_DRIVER_RISCOS)

/** RISC OS custom event structure */
struct SDL_SysWMmsg {
	SDL_version version;
	int eventCode;		/**< The window for the message */
	int pollBlock[64];
};

/** The RISC OS custom window manager information structure */
struct SDL_SysWMinfo {
	SDL_version version;
	int wimpVersion;    /**< Wimp version running under */
	int taskHandle;     /**< The RISC OS task handle */
	int window;		/**< The RISC OS display window */
};

#elif defined(SDL_VIDEO_DRIVER_PHOTON)

/** The QNX custom event structure */
struct SDL_SysWMmsg {
	SDL_version version;
	int data;
};

/** The QNX custom window manager information structure */
struct SDL_SysWMinfo {
	SDL_version version;
	int data;
};

#else

/** The generic custom event structure */
struct SDL_SysWMmsg {
	SDL_version version;
	int data;
};

/** The generic custom window manager information structure */
struct SDL_SysWMinfo {
	SDL_version version;
	int data;
};

#endif /* video driver type */

#endif /* SDL_PROTOTYPES_ONLY */

/* Function prototypes */
/**
 * This function gives you custom hooks into the window manager information.
 * It fills the structure pointed to by 'info' with custom information and
 * returns 0 if the function is not implemented, 1 if the function is 
 * implemented and no error occurred, and -1 if the version member of
 * the 'info' structure is not filled in or not supported.
 *
 * You typically use this function like this:
 * @code
 * SDL_SysWMinfo info;
 * SDL_VERSION(&info.version);
 * if ( SDL_GetWMInfo(&info) ) { ... }
 * @endcode
 */
extern DECLSPEC int SDLCALL SDL_GetWMInfo(SDL_SysWMinfo *info);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* _SDL_syswm_h */
