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
 * @file SDL.h
 * @name SDL_INIT Flags
 * @brief Main include header for the SDL library
 ******************************************************************************/
/*!*****************************************************************************
 * @defgroup main SDL
 * @brief Initialization and Shutdown
 * @details The functions in this category are used to set up SDL for use and
 * generally have global effects in your program.
 * @{
 ******************************************************************************/

#ifndef SDL_h_
#define SDL_h_

#include "SDL_main.h"
#include "SDL_stdinc.h"
#include "SDL_audio.h"
#include "SDL_cdrom.h"
#include "SDL_cpuinfo.h"
#include "SDL_endian.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_loadso.h"
#include "SDL_log.h"
#include "SDL_mutex.h"
#include "SDL_power.h"
#include "SDL_rwops.h"
#include "SDL_revision.h"
#include "SDL_pixels.h"
#include "SDL_platform.h"
#include "SDL_thread.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "SDL_version.h"
#include "SDL_filesystem.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * These are the flags which may be passed to SDL_Init() -- you should
 * specify the subsystems which you will be using in your application.
 */
#define SDL_INIT_TIMER        0x00000001
#define SDL_INIT_AUDIO        0x00000010
#define SDL_INIT_VIDEO        0x00000020 /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
#define SDL_INIT_JOYSTICK     0x00000200 /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
#define SDL_INIT_HAPTIC       0x00001000
#define SDL_INIT_CDROM        0x00000100
#define SDL_INIT_NOPARACHUTE  0x00100000 /**< Don't catch fatal signals */
#define SDL_INIT_EVENTTHREAD  0x01000000 /**< Not supported on all OS's */
#define SDL_INIT_EVERYTHING   ( SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC )

/**
 * This function loads the SDL dynamically linked library and initializes
 * the subsystems specified by 'flags' (and those satisfying dependencies)
 * Unless the SDL_INIT_NOPARACHUTE flag is set, it will install cleanup
 * signal handlers for some commonly ignored fatal signals (like SIGSEGV)
 */
extern DECLSPEC int SDLCALL SDL_Init(Uint32 flags);

/**
 * This function initializes specific SDL subsystems
 *
 * Subsystem initialization is ref-counted, you must call
 * SDL_QuitSubSystem() for each SDL_InitSubSystem() to correctly
 * shutdown a subsystem manually (or call SDL_Quit() to force shutdown).
 * If a subsystem is already loaded then this call will
 * increase the ref-count and return.
 */
extern DECLSPEC int SDLCALL SDL_InitSubSystem(Uint32 flags);

/**
 * This function cleans up specific SDL subsystems
 */
extern DECLSPEC void SDLCALL SDL_QuitSubSystem(Uint32 flags);

/**
 * This function returns a mask of the specified subsystems which have
 * previously been initialized.
 *
 * If \c flags is 0, it returns a mask of all initialized subsystems.
 */
extern DECLSPEC Uint32 SDLCALL SDL_WasInit(Uint32 flags);

/**
 * This function cleans up all initialized subsystems. You should
 * call it upon all exit conditions.
 */
extern DECLSPEC void SDLCALL SDL_Quit(void);
/*!@}*/

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_h_ */
