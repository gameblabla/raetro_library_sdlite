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
#ifndef math_libm_h_
#define math_libm_h_

#include "SDL_config.h"

/* Math routines from uClibc: http://www.uclibc.org */

double SDL_uclibc_atan(double x);
double SDL_uclibc_atan2(double y, double x);    
double SDL_uclibc_copysign(double x, double y);       
double SDL_uclibc_cos(double x);         
double SDL_uclibc_exp(double x);
double SDL_uclibc_fabs(double x);        
double SDL_uclibc_floor(double x);
double SDL_uclibc_fmod(double x, double y);
double SDL_uclibc_log(double x);
double SDL_uclibc_log10(double x);
double SDL_uclibc_pow(double x, double y);    
double SDL_uclibc_scalbn(double x, int n);
double SDL_uclibc_sin(double x);
double SDL_uclibc_sqrt(double x);
double SDL_uclibc_tan(double x);

#endif /* math_libm_h_ */

/* vi: set ts=4 sw=4 expandtab: */
