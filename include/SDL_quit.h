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

/*!*****************************************************************************
 * @file SDL_quit.h
 *
 * @brief SDL Quit.
 *
 * @details An SDL_QUITEVENT is generated when the user tries to close the
 * application window.
 * If it is ignored or filtered out, the window will remain open.
 * If it is not ignored or filtered, it is queued normally and the window
 * is allowed to close.  When the window is closed, screen updates will
 * complete, but have no effect.
 *
 * SDL_Init() installs signal handlers for SIGINT (keyboard interrupt)
 * and SIGTERM (system termination request), if handlers do not already
 * exist, that generate SDL_QUITEVENT events as well.  There is no way
 * to determine the cause of an SDL_QUITEVENT, but setting a signal
 * handler in your application will override the default generation of
 * quit events for that signal.
 *
 * There are no functions directly affecting the quit event
 *
 ******************************************************************************/
/*!*****************************************************************************
 * @defgroup quit Quit
 * @{
 ******************************************************************************/
#ifndef SDL_quit_h_
#define SDL_quit_h_

#include "SDL_stdinc.h"
#include "SDL_error.h"

#define SDL_QuitRequested() (SDL_PumpEvents(), SDL_PeepEvents(NULL,0,SDL_PEEKEVENT,SDL_QUITMASK))

#endif /* SDL_quit_h_ */
/*!@}*/
