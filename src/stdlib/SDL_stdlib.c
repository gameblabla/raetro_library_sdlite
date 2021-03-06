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
/* This file contains portable stdlib functions for SDL */

#include "SDL_stdinc.h"
#include <math.h>
#include "../libm/math_libm.h"

double SDL_atan(double x) {
#if defined(HAVE_ATAN)
	return atan(x);
#else
	return SDL_uclibc_atan(x);
#endif
}

float SDL_atanf(float x) {
#if defined(HAVE_ATANF)
	return atanf(x);
#else
	return (float) SDL_atan((double) x);
#endif
}

double SDL_atan2(double x, double y) {
#if defined(HAVE_ATAN2)
	return atan2(x, y);
#else
	return SDL_uclibc_atan2(x, y);
#endif
}

float SDL_atan2f(float x, float y) {
#if defined(HAVE_ATAN2F)
	return atan2f(x, y);
#else
	return (float) SDL_atan2((double) x, (double) y);
#endif
}

double SDL_acos(double val) {
#if defined(HAVE_ACOS)
	return acos(val);
#else
	double result;
	if(val == -1.0) {
		result = M_PI;
	} else {
		result = SDL_atan(SDL_sqrt(1.0 - val * val) / val);
		if(result < 0.0) {
			result += M_PI;
		}
	}
	return result;
#endif
}

float SDL_acosf(float val) {
#if defined(HAVE_ACOSF)
	return acosf(val);
#else
	return (float) SDL_acos((double) val);
#endif
}

double SDL_asin(double val) {
#if defined(HAVE_ASIN)
	return asin(val);
#else
	double result;
	if(val == -1.0) {
		result = -(M_PI / 2.0);
	} else {
		result = (M_PI / 2.0) - SDL_acos(val);
	}
	return result;
#endif
}

float SDL_asinf(float val) {
#if defined(HAVE_ASINF)
	return asinf(val);
#else
	return (float) SDL_asin((double) val);
#endif
}

double SDL_ceil(double x) {
#if defined(HAVE_CEIL)
	return ceil(x);
#else
	double integer = SDL_floor(x);
	double fraction = x - integer;
	if(fraction > 0.0) {
		integer += 1.0;
	}
	return integer;
#endif /* HAVE_CEIL */
}

float SDL_ceilf(float x) {
#if defined(HAVE_CEILF)
	return ceilf(x);
#else
	return (float) SDL_ceil((float) x);
#endif
}

double SDL_copysign(double x, double y) {
#if defined(HAVE_COPYSIGN)
	return copysign(x, y);
#elif defined(HAVE__COPYSIGN)
	return _copysign(x, y);
#elif defined(__WATCOMC__) && defined(__386__)
	/* this is nasty as hell, but it works.. */
	unsigned int *xi = (unsigned int *) &x,
				 *yi = (unsigned int *) &y;
	xi[1] = (yi[1] & 0x80000000) | (xi[1] & 0x7fffffff);
	return x;
#else
	return SDL_uclibc_copysign(x, y);
#endif /* HAVE_COPYSIGN */
}

float SDL_copysignf(float x, float y) {
#if defined(HAVE_COPYSIGNF)
	return copysignf(x, y);
#else
	return (float) SDL_copysign((double) x, (double) y);
#endif
}

double SDL_cos(double x) {
#if defined(HAVE_COS)
	return cos(x);
#else
	return SDL_uclibc_cos(x);
#endif
}

float SDL_cosf(float x) {
#if defined(HAVE_COSF)
	return cosf(x);
#else
	return (float) SDL_cos((double) x);
#endif
}

double SDL_exp(double x) {
#if defined(HAVE_EXP)
	return exp(x);
#else
	return SDL_uclibc_exp(x);
#endif
}

float SDL_expf(float x) {
#if defined(HAVE_EXPF)
	return expf(x);
#else
	return (float) SDL_exp((double) x);
#endif
}

double SDL_fabs(double x) {
#if defined(HAVE_FABS)
	return fabs(x);
#else
	return SDL_uclibc_fabs(x);
#endif
}

float SDL_fabsf(float x) {
#if defined(HAVE_FABSF)
	return fabsf(x);
#else
	return (float) SDL_fabs((double) x);
#endif
}

double SDL_floor(double x) {
#if defined(HAVE_FLOOR)
	return floor(x);
#else
	return SDL_uclibc_floor(x);
#endif
}

float SDL_floorf(float x) {
#if defined(HAVE_FLOORF)
	return floorf(x);
#else
	return (float) SDL_floor((double) x);
#endif
}

double SDL_trunc(double x) {
#if defined(HAVE_TRUNC)
	return trunc(x);
#else
	if(x >= 0.0f) {
		return SDL_floor(x);
	} else {
		return SDL_ceil(x);
	}
#endif
}

float SDL_truncf(float x) {
#if defined(HAVE_TRUNCF)
	return truncf(x);
#else
	return (float) SDL_trunc((double) x);
#endif
}

double SDL_fmod(double x, double y) {
#if defined(HAVE_FMOD)
	return fmod(x, y);
#else
	return SDL_uclibc_fmod(x, y);
#endif
}

float SDL_fmodf(float x, float y) {
#if defined(HAVE_FMODF)
	return fmodf(x, y);
#else
	return (float) SDL_fmod((double) x, (double) y);
#endif
}

double SDL_log(double x) {
#if defined(HAVE_LOG)
	return log(x);
#else
	return SDL_uclibc_log(x);
#endif
}

float SDL_logf(float x) {
#if defined(HAVE_LOGF)
	return logf(x);
#else
	return (float) SDL_log((double) x);
#endif
}

double SDL_log10(double x) {
#if defined(HAVE_LOG10)
	return log10(x);
#else
	return SDL_uclibc_log10(x);
#endif
}

float SDL_log10f(float x) {
#if defined(HAVE_LOG10F)
	return log10f(x);
#else
	return (float) SDL_log10((double) x);
#endif
}

double SDL_pow(double x, double y) {
#if defined(HAVE_POW)
	return pow(x, y);
#else
	return SDL_uclibc_pow(x, y);
#endif
}

float SDL_powf(float x, float y) {
#if defined(HAVE_POWF)
	return powf(x, y);
#else
	return (float) SDL_pow((double) x, (double) y);
#endif
}

double SDL_scalbn(double x, int n) {
#if defined(HAVE_SCALBN)
	return scalbn(x, n);
#elif defined(HAVE__SCALB)
	return _scalb(x, n);
#elif defined(HAVE_LIBC) && defined(HAVE_FLOAT_H) && (FLT_RADIX == 2)
	/* from scalbn(3): If FLT_RADIX equals 2 (which is
 * usual), then scalbn() is equivalent to ldexp(3). */
	return ldexp(x, n);
#else
	return SDL_uclibc_scalbn(x, n);
#endif
}

float SDL_scalbnf(float x, int n) {
#if defined(HAVE_SCALBNF)
	return scalbnf(x, n);
#else
	return (float) SDL_scalbn((double) x, n);
#endif
}

double SDL_sin(double x) {
#if defined(HAVE_SIN)
	return sin(x);
#else
	return SDL_uclibc_sin(x);
#endif
}

float SDL_sinf(float x) {
#if defined(HAVE_SINF)
	return sinf(x);
#else
	return (float) SDL_sin((double) x);
#endif
}

double SDL_sqrt(double x) {
#if defined(HAVE_SQRT)
	return sqrt(x);
#else
	return SDL_uclibc_sqrt(x);
#endif
}

float SDL_sqrtf(float x) {
#if defined(HAVE_SQRTF)
	return sqrtf(x);
#else
	return (float) SDL_sqrt((double) x);
#endif
}

double SDL_tan(double x) {
#if defined(HAVE_TAN)
	return tan(x);
#else
	return SDL_uclibc_tan(x);
#endif
}

float SDL_tanf(float x) {
#if defined(HAVE_TANF)
	return tanf(x);
#else
	return (float) SDL_tan((double) x);
#endif
}

int SDL_abs(int x) {
#if defined(HAVE_ABS)
	return abs(x);
#else
	return ((x) < 0 ? -(x) : (x));
#endif
}

#if defined(HAVE_CTYPE_H)

int SDL_isdigit(int x) {
	return isdigit(x);
}

int SDL_isspace(int x) {
	return isspace(x);
}

int SDL_isupper(int x) {
	return isupper(x);
}

int SDL_islower(int x) {
	return islower(x);
}

int SDL_toupper(int x) {
	return toupper(x);
}

int SDL_tolower(int x) {
	return tolower(x);
}

#else

int SDL_isdigit(int x) { return ((x) >= '0') && ((x) <= '9'); }

int SDL_isspace(int x) { return ((x) == ' ') || ((x) == '\t') || ((x) == '\r') || ((x) == '\n') || ((x) == '\f') || ((x) == '\v'); }

int SDL_isupper(int x) { return ((x) >= 'A') && ((x) <= 'Z'); }

int SDL_islower(int x) { return ((x) >= 'a') && ((x) <= 'z'); }

int SDL_toupper(int x) { return ((x) >= 'a') && ((x) <= 'z') ? ('A' + ((x) - 'a')) : (x); }

int SDL_tolower(int x) { return ((x) >= 'A') && ((x) <= 'Z') ? ('a' + ((x) - 'A')) : (x); }

#endif

#ifndef HAVE_LIBC

#endif /* !HAVE_LIBC */
