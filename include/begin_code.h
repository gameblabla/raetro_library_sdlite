/*******************************************************************************
 * Library       : SDLite 1.2.x
 * Purpose       : Low-level access to a framebuffer, audio output and HID.
 * Module        : Core
 * Project       : Redux for Embedded System
 * Description   : Stripped-down and optimized libraries for RISC processors
 * License       : GNU General Public License v3.0
 *******************************************************************************
 *
 * TinyRetroLabs and SDLite 1.2.x:
 * Copyright (c) 2019-2020 Marcus Andrade <boogermann@tinyretrolabs.org>
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
 * @file begin_code.h
 * @brief Header to set things up.
 *
 * This file sets things up for C dynamic library function definitions,
 * static inline functions, and structures aligned at 4-byte alignment.
 ******************************************************************************/

/**
 * This file shouldn't be nested -- included it around code only.
 */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif

/**
 * Header Definition
 */
#define _begin_code_h

/**
 * Some compilers use a special export keyword
 */
#ifndef DECLSPEC
#if defined(__GNUC__) && __GNUC__ >= 4
#define DECLSPEC __attribute__ ((visibility("default")))
#else
#define DECLSPEC
#endif
#endif

/** 
 * By default SDL uses the C calling convention
 */
#ifndef SDLCALL
#define SDLCALL
#endif /* SDLCALL */

/**
 * Set up compiler-specific options for inline functions
 */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#endif
#endif /* SDL_INLINE_OKAY */

/**
 * If inline isn't supported, remove "__inline__", turning static
 * inline functions into static functions (resulting in code bloat
 * in all files which include the offending header files)
 */
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif
