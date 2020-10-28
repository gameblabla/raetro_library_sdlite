/* include/SDL_config.h.  Generated from SDL_config.h.in by configure.  */
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
 * @file SDL_config.h.default
 * @brief Header with set of defines to configure the SDL features
 ******************************************************************************/

#ifndef SDL_config_h_
#define SDL_config_h_

/* General platform specific identifiers */
#include "SDL_platform.h"

/* C language features */
/* #undef const */
/* #undef inline */
/* #undef volatile */

/* C datatypes */
/* #undef size_t */
/* #undef int8_t */
/* #undef uint8_t */
/* #undef int16_t */
/* #undef uint16_t */
/* #undef int32_t */
/* #undef uint32_t */
/* #undef int64_t */
/* #undef uint64_t */
/* #undef uintptr_t */
#define SDL_HAS_64BIT_TYPE 1

/* Endianness */
#define SDL_BYTEORDER 1234

/* Comment this if you want to build without any C library requirements */
#define HAVE_LIBC 1
#if HAVE_LIBC

/* Useful headers */
#define STDC_HEADERS 1
#define HAVE_ALLOCA_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STDIO_H 1
#define STDC_HEADERS 1
#define HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1
#define HAVE_MALLOC_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_CTYPE_H 1
#define HAVE_MATH_H 1
#define HAVE_FLOAT_H 1
#define HAVE_ICONV_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_LIMITS_H 1
#define HAVE_MALLOC_H 1
#define HAVE_MATH_H 1
#define HAVE_MEMORY_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_STDARG_H 1
#define HAVE_UNISTD_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_FCNTL_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_WCHAR_H 1
/* #undef HAVE_PTHREAD_NP_H */
/* #undef HAVE_LIBUNWIND_H */

/* C library functions */
#define HAVE_MALLOC 1
#define HAVE_CALLOC 1
#define HAVE_REALLOC 1
#define HAVE_FREE 1
#define HAVE_ALLOCA 1
#define HAVE_QSORT 1
#define HAVE_ABS 1
#define HAVE_BCOPY 1
#define HAVE_MEMSET 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMCMP 1
#define HAVE_WCSLEN 1
/* #undef HAVE_WCSLCPY */
/* #undef HAVE_WCSLCAT */
#define HAVE_WCSDUP 1
#define HAVE_WCSSTR 1
#define HAVE_WCSCMP 1
#define HAVE_WCSNCMP 1
#define HAVE_STRLEN 1
/* #undef HAVE_STRLCPY */
/* #undef HAVE_STRLCAT */
#define HAVE_STRDUP 1
/* #undef HAVE__STRREV */
/* #undef HAVE__STRUPR */
/* #undef HAVE__STRLWR */
/* #undef HAVE_INDEX */
/* #undef HAVE_RINDEX */
#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1
#define HAVE_STRTOK_R 1
/* #undef HAVE_STRTOK_S */
/* #undef HAVE_ITOA */
/* #undef HAVE__LTOA */
/* #undef HAVE__UITOA */
/* #undef HAVE__ULTOA */
#define HAVE_STRTOL 1
#define HAVE_STRTOUL 1
/* #undef HAVE__I64TOA */
/* #undef HAVE__UI64TOA */
#define HAVE_STRTOLL 1
#define HAVE_STRTOULL 1
/* #undef HAVE__STRTOI64 */
/* #undef HAVE__STRTOUI64 */
#define HAVE_STRTOD 1
#define HAVE_ATOI 1
#define HAVE_ATOF 1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP 1
/* #undef HAVE__STRICMP */
#define HAVE_STRCASECMP 1
/* #undef HAVE__STRNICMP */
#define HAVE_STRNCASECMP 1
#define HAVE_SSCANF 1
#define HAVE_VSSCANF 1
#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1
/* #undef HAVE_M_PI */
/* #undef HAVE_ACOS */
/* #undef HAVE_ACOSF */
/* #undef HAVE_ASIN */
/* #undef HAVE_ASINF */
/* #undef HAVE_ATAN */
/* #undef HAVE_ATANF */
/* #undef HAVE_ATAN2 */
/* #undef HAVE_ATAN2F */
/* #undef HAVE_CEIL */
/* #undef HAVE_CEILF */
/* #undef HAVE_COPYSIGN */
/* #undef HAVE_COPYSIGNF */
/* #undef HAVE_COS */
/* #undef HAVE_COSF */
/* #undef HAVE_EXP */
/* #undef HAVE_EXPF */
/* #undef HAVE_FABS */
/* #undef HAVE_FABSF */
/* #undef HAVE_FLOOR */
/* #undef HAVE_FLOORF */
/* #undef HAVE_FMOD */
/* #undef HAVE_FMODF */
/* #undef HAVE_LOG */
/* #undef HAVE_LOGF */
/* #undef HAVE_LOG10 */
/* #undef HAVE_LOG10F */
/* #undef HAVE_POW */
/* #undef HAVE_POWF */
/* #undef HAVE_SCALBN */
/* #undef HAVE_SCALBNF */
/* #undef HAVE_SIN */
/* #undef HAVE_SINF */
/* #undef HAVE_SQRT */
/* #undef HAVE_SQRTF */
/* #undef HAVE_TAN */
/* #undef HAVE_TANF */
#define HAVE_FOPEN64 1
#define HAVE_FSEEKO 1
#define HAVE_FSEEKO64 1
#define HAVE_SIGACTION 1
#define HAVE_SA_SIGACTION 1
#define HAVE_SETJMP 1
#define HAVE_NANOSLEEP 1
#define HAVE_SYSCONF 1
/* #undef HAVE_SYSCTLBYNAME */
#define HAVE_CLOCK_GETTIME 1
#define HAVE_GETPAGESIZE 1
#define HAVE_MPROTECT 1
#define HAVE_ICONV 1
/* #undef HAVE_PTHREAD_SETNAME_NP */
/* #undef HAVE_PTHREAD_SET_NAME_NP */
#define HAVE_SEM_TIMEDWAIT 1
#define HAVE_GETAUXVAL 1
#define HAVE_POLL 1
#define HAVE__EXIT 1
#define HAVE_ICONV 1
/* #undef HAVE_OSMESA */

#else
#define HAVE_STDARG_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STDINT_H 1
#endif /* HAVE_LIBC */

/* #undef DEBUG */
/* #undef DEBUG_ASM */
/* #undef DEBUG_AUDIO */
/* #undef DEBUG_BUILD */
/* #undef DEBUG_CDROM */
/* #undef DEBUG_CONVERT */
/* #undef DEBUG_ERROR */
/* #undef DEBUG_FBCON */
/* #undef DEBUG_INPUT_EVENTS */
/* #undef DEBUG_KEYBOARD */
/* #undef DEBUG_MOUSE */
/* #undef DEBUG_PALETTE */
/* #undef DEBUG_PARAMS */
/* #undef DEBUG_QSORT */
/* #undef DEBUG_THREADS */
/* #undef DEBUG_TIMERS */
/* #undef DEBUG_VIDEO */

/* SDL internal assertion support */
/* #undef SDL_DEFAULT_ASSERT_LEVEL */

/* Allow disabling of core subsystems */
/* #undef SDL_AUDIO_DISABLED */
/* #undef SDL_CDROM_DISABLED */
/* #undef SDL_CPUINFO_DISABLED */
/* #undef SDL_EVENTS_DISABLED */
/* #undef SDL_FILE_DISABLED */
/* #undef SDL_JOYSTICK_DISABLED */
#define SDL_HAPTIC_DISABLED 1
/* #undef SDL_LOADSO_DISABLED */
/* #undef SDL_THREADS_DISABLED */
/* #undef SDL_TIMERS_DISABLED */
/* #undef SDL_VIDEO_DISABLED */
/* #undef SDL_POWER_DISABLED */
/* #undef SDL_FILESYSTEM_DISABLED */

/* Enable various audio drivers */
#define SDL_AUDIO_DRIVER_ALSA 1
/* #undef SDL_AUDIO_DRIVER_ALSA_DYNAMIC */
#define SDL_AUDIO_DRIVER_DUMMY 1
/* #undef SDL_AUDIO_DRIVER_PULSEAUDIO */
/* #undef SDL_AUDIO_DRIVER_PULSEAUDIO_DYNAMIC */
#define SDL_AUDIO_DRIVER_OSS 1
/* #undef SDL_AUDIO_DRIVER_OSS_SOUNDCARD_H */

/* Enable various cdrom drivers */
/* #undef SDL_CDROM_DUMMY */
#define SDL_CDROM_LINUX 1

/* Enable various input drivers */
#define SDL_INPUT_LINUXEV 1
/* #undef SDL_INPUT_LINUXKD */
/* #undef SDL_INPUT_TSLIB */
/* #undef SDL_JOYSTICK_DUMMY */
#define SDL_JOYSTICK_LINUX 1

/* Enable haptic subsystem drivers */
/* #undef SDL_HAPTIC_DUMMY */
/* #undef SDL_HAPTIC_IMMERSION */

/* Enable various shared object loading systems */
#define SDL_LOADSO_DLOPEN 1
/* #undef SDL_LOADSO_DUMMY */

/* Enable various threading systems */
/* #undef SDL_THREAD_PTH */
#define SDL_THREAD_PTHREAD 1
#define SDL_THREAD_PTHREAD_RECURSIVE_MUTEX 1
/* #undef SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP */

/* Enable various timer systems */
/* #undef SDL_TIMER_DUMMY */
#define SDL_TIMER_UNIX 1

/* Enable system power support */
#define SDL_POWER_LINUX 1

/* Enable various video drivers */
/* #undef SDL_VIDEO_DRIVER_DGA */
#define SDL_VIDEO_DRIVER_KMSDRM 1
#define SDL_VIDEO_DRIVER_DUMMY 1
#define SDL_VIDEO_DRIVER_FBCON 1

/* Enable OpenGL support */
/* #undef SDL_VIDEO_OPENGL */
/* #undef SDL_VIDEO_OPENGL_GLX */
/* #undef SDL_VIDEO_OPENGL_ES */
/* #undef SDL_VIDEO_OPENGL_OSMESA */
/* #undef SDL_VIDEO_OPENGL_OSMESA_DYNAMIC */
/* #undef NO_SDL_GLEXT */

/* Enable system filesystem support */
#define SDL_FILESYSTEM_UNIX 1

/* Disable screensaver */
#define SDL_VIDEO_DISABLE_SCREENSAVER 1

/* Enable assembly routines */
/* #undef SDL_ARM_SIMD_BLITTERS */
/* #undef SDL_ARM_NEON_BLITTERS */

/* Enable ime support */
/* #undef SDL_USE_IME */
/* Enable dynamic udev support */
/* #undef SDL_UDEV_DYNAMIC */
/* Enable dynamic libusb support */
/* #undef SDL_LIBUSB_DYNAMIC */
#endif /* SDL_config_h_ */
