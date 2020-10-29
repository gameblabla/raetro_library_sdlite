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
 * Copyright (c) 1997-2020 Sam Lantinga <slouken@libsdl.org>
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
#ifndef SDL_sysjoystick_c_h_
#define SDL_sysjoystick_c_h_

#include <linux/input.h>
#include <SDL_joystick.h>

struct SDL_joylist_item;

/* The private structure used to keep track of a joystick */
struct joystick_hwdata {
	int fd;
	struct SDL_joylist_item *item;
	SDL_JoystickGUID guid;
	char *fname;                /* Used in haptic subsystem */

	SDL_bool ff_rumble;
	SDL_bool ff_sine;
	struct ff_effect effect;
	Uint32 effect_expiration;

	/* The current Linux joystick driver maps hats to two axes */
	struct hwdata_hat {
		int axis[2];
	} *hats;
	/* The current Linux joystick driver maps balls to two axes */
	struct hwdata_ball {
		int axis[2];
	} *balls;

	/* Support for the Linux 2.4 unified input interface */
	SDL_bool is_hid;
	Uint8 key_map[KEY_MAX];
	Uint8 abs_map[ABS_MAX];
	SDL_bool has_key[KEY_MAX];
	SDL_bool has_abs[ABS_MAX];

	struct axis_correct {
		int used;
		int coef[3];
	} abs_correct[ABS_MAX];

	SDL_bool fresh;
	SDL_bool recovering_from_dropped;

	/* Steam Controller support */
	SDL_bool m_bSteamController;
	/* 4 = (ABS_HAT3X-ABS_HAT0X)/2 (see input-event-codes.h in kernel) */
	int hats_indices[4];
	SDL_bool has_hat[4];

	/* Set when gamepad is pending removal due to ENODEV read error */
	SDL_bool gone;
};

#endif /* SDL_sysjoystick_c_h_ */
