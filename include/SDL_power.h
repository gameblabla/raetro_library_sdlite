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
 * @file SDL_power.h
 * @brief Header for power management routines.
 ******************************************************************************/
/*!*****************************************************************************
 * @defgroup power Power Management
 * @details Power Management Status
 * @{
 ******************************************************************************/
#ifndef SDL_power_h_
#define SDL_power_h_

#include "SDL_stdinc.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/*!*****************************************************************************
 * @brief The basic state for the system's power supply.
 ******************************************************************************/
typedef enum {
	SDL_POWERSTATE_UNKNOWN,      /**< cannot determine power status */
	SDL_POWERSTATE_ON_BATTERY,   /**< Not plugged in, running on the battery */
	SDL_POWERSTATE_NO_BATTERY,   /**< Plugged in, no battery available */
	SDL_POWERSTATE_CHARGING,     /**< Plugged in, charging battery */
	SDL_POWERSTATE_CHARGED       /**< Plugged in, battery charged */
} SDL_PowerState;

/*!*****************************************************************************
 * @brief Get the current power supply details.
 *
 * @param secs Seconds of battery life left. You can pass a NULL here if
 *              you don't care. Will return -1 if we can't determine a
 *              value, or we're not running on a battery.
 *
 * @param pct Percentage of battery life left, between 0 and 100. You can
 *             pass a NULL here if you don't care. Will return -1 if we
 *             can't determine a value, or we're not running on a battery.
 *
 * @return The state of the battery (if any).
 ******************************************************************************/
extern DECLSPEC SDL_PowerState SDLCALL SDL_GetPowerInfo(int *secs, int *pct);
/*!@}*/

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_power_h_ */
