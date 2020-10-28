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
/**
 * @file SDL_joystick.h
 * @note In order to use these functions, SDL_Init() must have been called
 *       with the SDL_INIT_JOYSTICK flag. This causes SDL to scan the system
 *       for joysticks, and load appropriate drivers.
 */
#ifndef SDL_joystick_h_
#define SDL_joystick_h_

#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif


/** The joystick structure used to identify an SDL joystick */
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

/* A structure that encodes the stable unique id for a joystick device */
typedef struct {
	Uint8 data[16];
} SDL_JoystickGUID;

typedef Sint32 SDL_JoystickID;

typedef enum {
	SDL_JOYSTICK_TYPE_UNKNOWN,
	SDL_JOYSTICK_TYPE_GAMECONTROLLER,
	SDL_JOYSTICK_TYPE_WHEEL,
	SDL_JOYSTICK_TYPE_ARCADE_STICK,
	SDL_JOYSTICK_TYPE_FLIGHT_STICK,
	SDL_JOYSTICK_TYPE_DANCE_PAD,
	SDL_JOYSTICK_TYPE_GUITAR,
	SDL_JOYSTICK_TYPE_DRUM_KIT,
	SDL_JOYSTICK_TYPE_ARCADE_PAD,
	SDL_JOYSTICK_TYPE_THROTTLE
} SDL_JoystickType;

typedef enum {
	SDL_JOYSTICK_POWER_UNKNOWN = -1,
	SDL_JOYSTICK_POWER_EMPTY,        /* <= 5% */
	SDL_JOYSTICK_POWER_LOW,          /* <= 20% */
	SDL_JOYSTICK_POWER_MEDIUM,       /* <= 70% */
	SDL_JOYSTICK_POWER_FULL,         /* <= 100% */
	SDL_JOYSTICK_POWER_WIRED,
	SDL_JOYSTICK_POWER_MAX
} SDL_JoystickPowerLevel;


/* Function prototypes */
/**
 * Count the number of joysticks attached to the system
 */
extern DECLSPEC int SDLCALL SDL_NumJoysticks(void);

/**
 * Get the implementation dependent name of a joystick.
 *
 * This can be called before any joysticks are opened.
 * If no name can be found, this function returns NULL.
 */
extern DECLSPEC const char *SDLCALL SDL_JoystickName(int device_index);

/**
 * Open a joystick for use.
 *
 * @param[in] device_index
 * The index passed as an argument refers to
 * the N'th joystick on the system.  This index is the value which will
 * identify this joystick in future joystick events.
 *
 * @return This function returns a joystick identifier, or NULL if an error occurred.
 */
extern DECLSPEC SDL_Joystick *SDLCALL SDL_JoystickOpen(int device_index);

/**
 * Returns 1 if the joystick has been opened, or 0 if it has not.
 */
extern DECLSPEC int SDLCALL SDL_JoystickOpened(int device_index);

/**
 * Get the device index of an opened joystick.
 */
extern DECLSPEC int SDLCALL SDL_JoystickIndex(SDL_Joystick *joystick);

/**
 * Get the number of general axis controls on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumAxes(SDL_Joystick *joystick);

/**
 * Get the number of trackballs on a joystick
 *
 * Joystick trackballs have only relative motion events associated
 * with them and their state cannot be polled.
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumBalls(SDL_Joystick *joystick);

/**
 * Get the number of POV hats on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumHats(SDL_Joystick *joystick);

/**
 * Get the number of buttons on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumButtons(SDL_Joystick *joystick);

/**
 * Update the current state of the open joysticks.
 *
 * This is called automatically by the event loop if any joystick
 * events are enabled.
 */
extern DECLSPEC void SDLCALL SDL_JoystickUpdate(void);

/**
 * Enable/disable joystick event polling.
 *
 * If joystick events are disabled, you must call SDL_JoystickUpdate()
 * yourself and check the state of the joystick when you want joystick
 * information.
 *
 * @param[in] state The state can be one of SDL_QUERY, SDL_ENABLE or SDL_IGNORE.
 */
extern DECLSPEC int SDLCALL SDL_JoystickEventState(int state);

/**
 * Get the current state of an axis control on a joystick
 *
 * @param[in] axis The axis indices start at index 0.
 *
 * @return The state is a value ranging from -32768 to 32767.
 */
extern DECLSPEC Sint16 SDLCALL SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis);

/**
 *  @name Hat Positions
 *  The return value of SDL_JoystickGetHat() is one of the following positions:
 */
/*@{*/
#define SDL_HAT_CENTERED  0x00
#define SDL_HAT_UP        0x01
#define SDL_HAT_RIGHT     0x02
#define SDL_HAT_DOWN      0x04
#define SDL_HAT_LEFT      0x08
#define SDL_HAT_RIGHTUP   (SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN (SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP    (SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN  (SDL_HAT_LEFT|SDL_HAT_DOWN)
/*@}*/

/** 
 *  Get the current state of a POV hat on a joystick
 *
 *  @param[in] hat The hat indices start at index 0.
 */
extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetHat(SDL_Joystick *joystick, int hat);

/**
 * Get the ball axis change since the last poll
 *
 * @param[in] ball The ball indices start at index 0.
 *
 * @return This returns 0, or -1 if you passed it invalid parameters.
 */
extern DECLSPEC int SDLCALL SDL_JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy);

/**
 * Get the current state of a button on a joystick
 *
 * @param[in] button The button indices start at index 0.
 */
extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetButton(SDL_Joystick *joystick, int button);

/**
 * Close a joystick previously opened with SDL_JoystickOpen()
 */
extern DECLSPEC void SDLCALL SDL_JoystickClose(SDL_Joystick *joystick);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_joystick_h_ */
