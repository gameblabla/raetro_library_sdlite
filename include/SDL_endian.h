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
 *  @file SDL_endian.h
 *  Functions for reading and writing endian-specific values
 */

#ifndef SDL_endian_h_
#define SDL_endian_h_

#include "SDL_stdinc.h"

/** @name SDL_ENDIANs
 *  The two types of endianness 
 */
/*@{*/
#define SDL_LIL_ENDIAN    1234
#define SDL_BIG_ENDIAN    4321
/*@}*/

#ifndef SDL_BYTEORDER    /* Not defined in SDL_config.h? */
#ifdef __linux__

#include <endian.h>

#define SDL_BYTEORDER  __BYTE_ORDER
#else
#if (defined(__MIPS__) && defined(__MIPSEB__))
#define SDL_BYTEORDER	SDL_BIG_ENDIAN
#else
#define SDL_BYTEORDER	SDL_LIL_ENDIAN
#endif
#endif /* __linux __ */
#endif /* !SDL_BYTEORDER */

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @name SDL_Swap Functions
 *  Use inline functions for compilers that support them, and static
 *  functions for those that do not.  Because these functions become
 *  static for compilers that do not support inline functions, this
 *  header should only be included in files that actually use them.
 */
/*@{*/
#if defined(__GNUC__) && defined(__i386__) && !(__GNUC__ == 2 && __GNUC_MINOR__ <= 95 /* broken gcc version */)
static __inline__ Uint16 SDL_Swap16(Uint16 x) {
	__asm__("xchgb %b0,%h0" : "=q" (x) :  "0" (x));
	return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __inline__ Uint16 SDL_Swap16(Uint16 x) {
	__asm__("xchgb %b0,%h0" : "=Q" (x) :  "0" (x));
	return x;
}
#endif

#if defined(__GNUC__) && defined(__i386__) && !(__GNUC__ == 2 && __GNUC_MINOR__ <= 95 /* broken gcc version */)
static __inline__ Uint32 SDL_Swap32(Uint32 x) {
	__asm__("bswap %0" : "=r" (x) : "0" (x));
	return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __inline__ Uint32 SDL_Swap32(Uint32 x) {
	__asm__("bswapl %0" : "=r" (x) : "0" (x));
	return x;
}
#endif

#ifdef SDL_HAS_64BIT_TYPE
#if defined(__GNUC__) && defined(__i386__) && \
   !(__GNUC__ == 2 && __GNUC_MINOR__ <= 95 /* broken gcc version */)
static __inline__ Uint64 SDL_Swap64(Uint64 x)
{
	union { 
		struct { Uint32 a,b; } s;
		Uint64 u;
	} v;
	v.u = x;
	__asm__("bswapl %0 ; bswapl %1 ; xchgl %0,%1" 
			: "=r" (v.s.a), "=r" (v.s.b)
			: "0" (v.s.a), "1" (v.s.b));
	return v.u;
}
#elif defined(__GNUC__) && defined(__x86_64__)
static __inline__ Uint64 SDL_Swap64(Uint64 x)
{
	__asm__("bswapq %0" : "=r" (x) : "0" (x));
	return x;
}
#else
static __inline__ Uint64 SDL_Swap64(Uint64 x)
{
	Uint32 hi, lo;

	/* Separate into high and low 32-bit values and swap them */
	lo = SDL_static_cast(Uint32, x & 0xFFFFFFFF);
	x >>= 32;
	hi = SDL_static_cast(Uint32, x & 0xFFFFFFFF);
	x = SDL_Swap32(lo);
	x <<= 32;
	x |= SDL_Swap32(hi);
	return (x);
}
#endif
#else
/* This is mainly to keep compilers from complaining in SDL code.
 * If there is no real 64-bit datatype, then compilers will complain about
 * the fake 64-bit datatype that SDL provides when it compiles user code.
 */
#define SDL_Swap64(X)    (X)
#endif /* SDL_HAS_64BIT_TYPE */
/*@}*/

/**
 *  @name SDL_SwapLE and SDL_SwapBE Functions
 *  Byteswap item from the specified endianness to the native endianness
 */
/*@{*/
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SDL_SwapLE16(X)    (X)
#define SDL_SwapLE32(X)    (X)
#define SDL_SwapLE64(X)    (X)
#define SDL_SwapBE16(X)    SDL_Swap16(X)
#define SDL_SwapBE32(X)    SDL_Swap32(X)
#define SDL_SwapBE64(X)    SDL_Swap64(X)
#else
#define SDL_SwapLE16(X)	SDL_Swap16(X)
#define SDL_SwapLE32(X)	SDL_Swap32(X)
#define SDL_SwapLE64(X)	SDL_Swap64(X)
#define SDL_SwapBE16(X)	(X)
#define SDL_SwapBE32(X)	(X)
#define SDL_SwapBE64(X)	(X)
#endif
/*@}*/

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_endian_h_ */
