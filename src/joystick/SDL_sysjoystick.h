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

#ifndef SDL_sysjoystick_h_
#define SDL_sysjoystick_h_

/* This is the system specific header for the SDL joystick API */

#include "SDL_joystick.h"
#include "SDL_joystick_c.h"

/* The SDL joystick structure */
typedef struct _SDL_JoystickAxisInfo {
	Sint16 initial_value;       /* Initial axis state */
	Sint16 value;               /* Current axis state */
	Sint16 zero;                /* Zero point on the axis (-32768 for triggers) */
	SDL_bool has_initial_value; /* Whether we've seen a value on the axis yet */
	SDL_bool has_second_value;  /* Whether we've seen a second value on the axis yet */
	SDL_bool sent_initial_value; /* Whether we've sent the initial axis value */
} SDL_JoystickAxisInfo;

struct _SDL_Joystick {
	Uint8 index;                         /* Device index */
	const char *name;                    /* Joystick name - system dependent */

	int naxes;                           /* Number of axis controls on the joystick */
	Sint16 *axes;                        /* Current axis states */

	int nhats;                           /* Number of hats on the joystick */
	Uint8 *hats;                         /* Current hat states */

	int nballs;                          /* Number of trackballs on the joystick */
	struct balldelta {
		int dx;
		int dy;
	} *balls;                            /* Current ball motion deltas */

	int nbuttons;                        /* Number of buttons on the joystick */
	Uint8 *buttons;                      /* Current button states */

	Uint16 low_frequency_rumble;
	Uint16 high_frequency_rumble;
	Uint32 rumble_expiration;

	SDL_bool attached;
	SDL_bool is_game_controller;
	SDL_bool delayed_guide_button;       /* SDL_TRUE if this device has the guide button event delayed */
	SDL_JoystickPowerLevel epowerlevel;  /* power level of this joystick, SDL_JOYSTICK_POWER_UNKNOWN if not supported */
	struct _SDL_JoystickDriver *driver;

	struct joystick_hwdata *hwdata;      /* Driver dependent information */

	int ref_count;                       /* Reference count for multiple opens */
	struct _SDL_Joystick *next;          /* pointer to next joystick we have allocated */
};

/* Function to scan the system for joysticks.
 * Joystick 0 should be the system default joystick.
 * This function should return the number of available joysticks, or -1
 * on an unrecoverable fatal error.
 */
extern int SDL_SYS_JoystickInit(void);

/* Function to get the device-dependent name of a joystick */
extern const char *SDL_SYS_JoystickName(int index);

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
extern int SDL_SYS_JoystickOpen(SDL_Joystick *joystick);

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
extern void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick);

/* Function to close a joystick after use */
extern void SDL_SYS_JoystickClose(SDL_Joystick *joystick);

/* Function to perform any system-specific joystick related cleanup */
extern void SDL_SYS_JoystickQuit(void);

/**
 * @brief
 */
/* Device bus definitions */
#define SDL_HARDWARE_BUS_USB        0x03
#define SDL_HARDWARE_BUS_BLUETOOTH  0x05

/* Macro to combine a USB vendor ID and product ID into a single Uint32 value */
#define MAKE_VIDPID(VID, PID)   (((Uint32)(VID))<<16|(PID))

typedef struct _SDL_JoystickDriver {
	/* Function to scan the system for joysticks.
	 * Joystick 0 should be the system default joystick.
	 * This function should return 0, or -1 on an unrecoverable error.
	 */
	int (*Init)(void);

	/* Function to return the number of joystick devices plugged in right now */
	int (*GetCount)(void);

	/* Function to cause any queued joystick insertions to be processed */
	void (*Detect)(void);

	/* Function to get the device-dependent name of a joystick */
	const char *(*GetDeviceName)(int device_index);

	/* Function to get the player index of a joystick */
	int (*GetDevicePlayerIndex)(int device_index);

	/* Function to get the player index of a joystick */
	void (*SetDevicePlayerIndex)(int device_index, int player_index);

	/* Function to return the stable GUID for a plugged in device */
	SDL_JoystickGUID (*GetDeviceGUID)(int device_index);

	/* Function to get the current instance id of the joystick located at device_index */
	SDL_JoystickID (*GetDeviceInstanceID)(int device_index);

	/* Function to open a joystick for use.
	   The joystick to open is specified by the device index.
	   This should fill the nbuttons and naxes fields of the joystick structure.
	   It returns 0, or -1 if there is an error.
	 */
	int (*Open)(SDL_Joystick *joystick, int device_index);

	/* Rumble functionality */
	int (*Rumble)(SDL_Joystick *joystick, Uint16 low_frequency_rumble, Uint16 high_frequency_rumble);

	/* Function to update the state of a joystick - called as a device poll.
	 * This function shouldn't update the joystick structure directly,
	 * but instead should call SDL_PrivateJoystick*() to deliver events
	 * and update joystick device state.
	 */
	void (*Update)(SDL_Joystick *joystick);

	/* Function to close a joystick after use */
	void (*Close)(SDL_Joystick *joystick);

	/* Function to perform any system-specific joystick related cleanup */
	void (*Quit)(void);

	/* Function to get the autodetected controller mapping; returns false if there isn't any. */
	SDL_bool (*GetGamepadMapping)(int device_index, SDL_GamepadMapping *out);

} SDL_JoystickDriver;

/* Windows and Mac OSX has a limit of MAX_DWORD / 1000, Linux kernel has a limit of 0xFFFF */
#define SDL_MAX_RUMBLE_DURATION_MS  0xFFFF

extern SDL_JoystickDriver SDL_DUMMY_JoystickDriver;
extern SDL_JoystickDriver SDL_LINUX_JoystickDriver;

#endif /* SDL_sysjoystick_h_ */
