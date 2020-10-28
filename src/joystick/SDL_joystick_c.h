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

/* Useful functions and variables from SDL_joystick.c */
#include "SDL_joystick.h"

/* The number of available joysticks on the system */
extern Uint8 SDL_numjoysticks;

/* Internal event queueing functions */
extern int SDL_PrivateJoystickAxis(SDL_Joystick *joystick, Uint8 axis, Sint16 value);

extern int SDL_PrivateJoystickBall(SDL_Joystick *joystick, Uint8 ball, Sint16 xrel, Sint16 yrel);

extern int SDL_PrivateJoystickHat(SDL_Joystick *joystick, Uint8 hat, Uint8 value);

extern int SDL_PrivateJoystickButton(SDL_Joystick *joystick, Uint8 button, Uint8 state);
