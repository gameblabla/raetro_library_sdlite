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

/** @file SDL_stdinc.h
 *  This is a general header that includes C language support
 */

#ifndef SDL_stdinc_h_
#define SDL_stdinc_h_

#include "SDL_config.h"

#ifdef HAVE_SYS_TYPES_H

#include <sys/types.h>

#endif
#ifdef HAVE_STDIO_H

#include <stdio.h>

#endif

#if defined(STDC_HEADERS)

# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>

#else
# if defined(HAVE_STDLIB_H)
#  include <stdlib.h>
# elif defined(HAVE_MALLOC_H)
#  include <malloc.h>
# endif
# if defined(HAVE_STDDEF_H)
#  include <stddef.h>
# endif
# if defined(HAVE_STDARG_H)
#  include <stdarg.h>
# endif
#endif

#ifdef HAVE_STRING_H
# if !defined(STDC_HEADERS) && defined(HAVE_MEMORY_H)
#  include <memory.h>
# endif

# include <string.h>

#endif

#ifdef HAVE_STRINGS_H

# include <strings.h>

#endif

#if defined(HAVE_INTTYPES_H)

# include <inttypes.h>

#elif defined(HAVE_STDINT_H)
# include <stdint.h>
#endif

#ifdef HAVE_CTYPE_H

# include <ctype.h>

#endif

#if defined(HAVE_ICONV) && defined(HAVE_ICONV_H)

# include <iconv.h>

#endif

#if defined(HAVE_ALLOCA) && !defined(alloca)
# if defined(HAVE_ALLOCA_H)
#  include <alloca.h>
# elif defined(__GNUC__)
#  define alloca __builtin_alloca
# else
char *alloca ();
# endif
#endif

/**
 *  The number of elements in an array.
 */
#define SDL_arraysize(array)    (sizeof(array)/sizeof(array[0]))
#define SDL_TABLESIZE(table)    SDL_arraysize(table)

/**
 *  Macro useful for building other macros with strings in them
 *
 *  e.g. #define LOG_ERROR(X) OutputDebugString(SDL_STRINGIFY_ARG(__FUNCTION__) ": " X "\n")
 */
#define SDL_STRINGIFY_ARG(arg)  #arg

/**
 *  \name Cast operators
 *
 *  Use proper C++ casts when compiled as C++ to be compatible with the option
 *  -Wold-style-cast of GCC (and -Werror=old-style-cast in GCC 4.2 and above).
 */
/* @{ */
#ifdef __cplusplus
#define SDL_reinterpret_cast(type, expression) reinterpret_cast<type>(expression)
#define SDL_static_cast(type, expression) static_cast<type>(expression)
#define SDL_const_cast(type, expression) const_cast<type>(expression)
#else
#define SDL_reinterpret_cast(type, expression) ((type)(expression))
#define SDL_static_cast(type, expression) ((type)(expression))
#define SDL_const_cast(type, expression) ((type)(expression))
#endif
/* @} *//* Cast operators */



/* @{ */

#ifdef __CC_ARM
/* ARM's compiler throws warnings if we use an enum: like "SDL_bool x = a < b;" */
#define SDL_FALSE 0
#define SDL_TRUE 1
typedef int SDL_bool;
#else
typedef enum {
	SDL_FALSE = 0,
	SDL_TRUE = 1
} SDL_bool;
#endif

/**
 * \brief A signed 8-bit integer type.
 */
#define SDL_MAX_SINT8   ((Sint8)0x7F)           /* 127 */
#define SDL_MIN_SINT8   ((Sint8)(~0x7F))        /* -128 */
typedef int8_t Sint8;
/**
 * \brief An unsigned 8-bit integer type.
 */
#define SDL_MAX_UINT8   ((Uint8)0xFF)           /* 255 */
#define SDL_MIN_UINT8   ((Uint8)0x00)           /* 0 */
typedef uint8_t Uint8;
/**
 * \brief A signed 16-bit integer type.
 */
#define SDL_MAX_SINT16  ((Sint16)0x7FFF)        /* 32767 */
#define SDL_MIN_SINT16  ((Sint16)(~0x7FFF))     /* -32768 */
typedef int16_t Sint16;
/**
 * \brief An unsigned 16-bit integer type.
 */
#define SDL_MAX_UINT16  ((Uint16)0xFFFF)        /* 65535 */
#define SDL_MIN_UINT16  ((Uint16)0x0000)        /* 0 */
typedef uint16_t Uint16;
/**
 * \brief A signed 32-bit integer type.
 */
#define SDL_MAX_SINT32  ((Sint32)0x7FFFFFFF)    /* 2147483647 */
#define SDL_MIN_SINT32  ((Sint32)(~0x7FFFFFFF)) /* -2147483648 */
typedef int32_t Sint32;
/**
 * \brief An unsigned 32-bit integer type.
 */
#define SDL_MAX_UINT32  ((Uint32)0xFFFFFFFFu)   /* 4294967295 */
#define SDL_MIN_UINT32  ((Uint32)0x00000000)    /* 0 */
typedef uint32_t Uint32;

#ifdef SDL_HAS_64BIT_TYPE
/**
 * \brief A signed 64-bit integer type.
 */
#define SDL_MAX_SINT64  ((Sint64)0x7FFFFFFFFFFFFFFFll)      /* 9223372036854775807 */
#define SDL_MIN_SINT64  ((Sint64)(~0x7FFFFFFFFFFFFFFFll))   /* -9223372036854775808 */
typedef int64_t Sint64;
/**
 * \brief An unsigned 64-bit integer type.
 */
#define SDL_MAX_UINT64  ((Uint64)0xFFFFFFFFFFFFFFFFull)     /* 18446744073709551615 */
#define SDL_MIN_UINT64  ((Uint64)(0x0000000000000000ull))   /* 0 */
typedef uint64_t Uint64;
#else
/* This is really just a hack to prevent the compiler from complaining */
typedef struct {
	Uint32 hi;
	Uint32 lo;
} Uint64, Sint64;
#endif
/* @} *//* Basic data types */

/* Annotations to help code analysis tools */
#ifdef SDL_DISABLE_ANALYZE_MACROS
#define SDL_IN_BYTECAP(x)
#define SDL_INOUT_Z_CAP(x)
#define SDL_OUT_Z_CAP(x)
#define SDL_OUT_CAP(x)
#define SDL_OUT_BYTECAP(x)
#define SDL_OUT_Z_BYTECAP(x)
#define SDL_PRINTF_FORMAT_STRING
#define SDL_SCANF_FORMAT_STRING
#define SDL_PRINTF_VARARG_FUNC( fmtargnumber )
#define SDL_SCANF_VARARG_FUNC( fmtargnumber )
#else
#define SDL_IN_BYTECAP(x)
#define SDL_INOUT_Z_CAP(x)
#define SDL_OUT_Z_CAP(x)
#define SDL_OUT_CAP(x)
#define SDL_OUT_BYTECAP(x)
#define SDL_OUT_Z_BYTECAP(x)
#define SDL_PRINTF_FORMAT_STRING
#define SDL_SCANF_FORMAT_STRING
#if defined(__GNUC__)
#define SDL_PRINTF_VARARG_FUNC(fmtargnumber) __attribute__ (( format( __printf__, fmtargnumber, fmtargnumber+1 )))
#define SDL_SCANF_VARARG_FUNC(fmtargnumber) __attribute__ (( format( __scanf__, fmtargnumber, fmtargnumber+1 )))
#else
#define SDL_PRINTF_VARARG_FUNC( fmtargnumber )
#define SDL_SCANF_VARARG_FUNC( fmtargnumber )
#endif
#endif /* SDL_DISABLE_ANALYZE_MACROS */


/** @name Make sure the types really have the right sizes */
/*@{*/
#define SDL_COMPILE_TIME_ASSERT(name, x)               \
       typedef int SDL_dummy_ ## name[(x) * 2 - 1]

SDL_COMPILE_TIME_ASSERT(uint8, sizeof(Uint8) == 1);
SDL_COMPILE_TIME_ASSERT(sint8, sizeof(Sint8) == 1);
SDL_COMPILE_TIME_ASSERT(uint16, sizeof(Uint16) == 2);
SDL_COMPILE_TIME_ASSERT(sint16, sizeof(Sint16) == 2);
SDL_COMPILE_TIME_ASSERT(uint32, sizeof(Uint32) == 4);
SDL_COMPILE_TIME_ASSERT(sint32, sizeof(Sint32) == 4);
SDL_COMPILE_TIME_ASSERT(uint64, sizeof(Uint64) == 8);
SDL_COMPILE_TIME_ASSERT(sint64, sizeof(Sint64) == 8);
/*@}*/

/** @name Enum Size Check
 *  Check to make sure enums are the size of ints, for structure packing.
 *  For both Watcom C/C++ and Borland C/C++ the compiler option that makes
 *  enums having the size of an int must be enabled.
 *  This is "-b" for Borland C/C++ and "-ei" for Watcom C/C++ (v11).
 */
/* Enable enums always int in CodeWarrior (for MPW use "-enum int") */

typedef enum {
	DUMMY_ENUM_VALUE
} SDL_DUMMY_ENUM;

/*@{*/
SDL_COMPILE_TIME_ASSERT(enum, sizeof(SDL_DUMMY_ENUM) == sizeof(int));
/*@}*/

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_MALLOC
#define SDL_malloc    malloc
#else

extern DECLSPEC void *SDLCALL SDL_malloc(size_t size);

#endif

#ifdef HAVE_CALLOC
#define SDL_calloc    calloc
#else

extern DECLSPEC void *SDLCALL SDL_calloc(size_t nmemb, size_t size);

#endif

#ifdef HAVE_REALLOC
#define SDL_realloc    realloc
#else

extern DECLSPEC void *SDLCALL SDL_realloc(void *mem, size_t size);

#endif

#ifdef HAVE_FREE
#define SDL_free    free
#else

extern DECLSPEC void SDLCALL SDL_free(void *mem);

#endif

#ifdef HAVE_ALLOCA
#define SDL_stack_alloc(type, count) (type*)alloca(sizeof(type)*(count))
#define SDL_stack_free(data)
#else
#define SDL_stack_alloc(type, count) (type*)SDL_malloc(sizeof(type)*(count))
#define SDL_stack_free(data) SDL_free(data)
#endif

#ifdef HAVE_GETENV
#define SDL_getenv	getenv
#else

extern DECLSPEC char *SDLCALL SDL_getenv(const char *name);

#endif

#ifdef HAVE_PUTENV
#define SDL_putenv	putenv
#else

extern DECLSPEC int SDLCALL SDL_putenv(const char *variable);

#endif

#ifdef HAVE_QSORT
#define SDL_qsort    qsort
#else

extern DECLSPEC void SDLCALL SDL_qsort(void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));

#endif

#ifdef HAVE_ABS
#define SDL_abs        abs
#else
#define SDL_abs(X)    ((X) < 0 ? -(X) : (X))
#endif

#define SDL_min(x, y)    (((x) < (y)) ? (x) : (y))
#define SDL_max(x, y)    (((x) > (y)) ? (x) : (y))


/**
 * @brief
 *
 */
extern DECLSPEC int SDLCALL SDL_isdigit(int x);
extern DECLSPEC int SDLCALL SDL_isspace(int x);
extern DECLSPEC int SDLCALL SDL_isupper(int x);
extern DECLSPEC int SDLCALL SDL_islower(int x);
extern DECLSPEC int SDLCALL SDL_toupper(int x);
extern DECLSPEC int SDLCALL SDL_tolower(int x);


//#ifdef HAVE_CTYPE_H
//#define SDL_isdigit(X)  isdigit(X)
//#define SDL_isspace(X)  isspace(X)
//#define SDL_toupper(X)  toupper(X)
//#define SDL_tolower(X)  tolower(X)
//#else
//#define SDL_isdigit(X)  (((X) >= '0') && ((X) <= '9'))
//#define SDL_isspace(X)  (((X) == ' ') || ((X) == '\t') || ((X) == '\r') || ((X) == '\n'))
//#define SDL_toupper(X)  (((X) >= 'a') && ((X) <= 'z') ? ('A'+((X)-'a')) : (X))
//#define SDL_tolower(X)  (((X) >= 'A') && ((X) <= 'Z') ? ('a'+((X)-'A')) : (X))
//#endif

#ifdef HAVE_MEMSET
#define SDL_memset      memset
#else

extern DECLSPEC void *SDLCALL SDL_memset(void *dst, int c, size_t len);

#endif

#ifndef SDL_memset4
#define SDL_memset4(dst, val, len)               \
do {                                             \
    unsigned _count = (len);                     \
    unsigned _n = (_count + 3) / 4;              \
    Uint32 *_p = SDL_static_cast(Uint32 *, dst); \
    Uint32 _val = (val);                         \
    if (len == 0) break;                         \
        switch (_count % 4) {                    \
        case 0: do {    *_p++ = _val;            \
        case 3:         *_p++ = _val;            \
        case 2:         *_p++ = _val;            \
        case 1:         *_p++ = _val;            \
        } while ( --_n );                        \
    }                                            \
} while(0)
#endif

/* We can count on memcpy existing on Mac OS X and being well-tuned. */
#ifndef SDL_memcpy
#ifdef HAVE_MEMCPY
#define SDL_memcpy      memcpy
#elif defined(HAVE_BCOPY)
#define SDL_memcpy(d, s, n)	bcopy((s), (d), (n))
#else

extern DECLSPEC void *SDLCALL SDL_memcpy(void *dst, const void *src, size_t len);

#endif
#endif

/* We can count on memcpy existing on Mac OS X and being well-tuned. */
#ifndef SDL_memcpy4
#define SDL_memcpy4(dst, src, len)    SDL_memcpy(dst, src, (len) << 2)
#endif

#ifndef SDL_revcpy

extern DECLSPEC void *SDLCALL SDL_revcpy(void *dst, const void *src, size_t len);

#endif

#ifdef HAVE_MEMMOVE
#define SDL_memmove     memmove
#elif defined(HAVE_BCOPY)
#define SDL_memmove(d, s, n)	bcopy((s), (d), (n))
#else
#define SDL_memmove(dst, src, len) do { \
    if ( dst < src ) {                \
        SDL_memcpy(dst, src, len);        \
    } else {                    \
        SDL_revcpy(dst, src, len);        \
    }                        \
} while(0)
#endif

#ifdef HAVE_MEMCMP
#define SDL_memcmp      memcmp
#else

extern DECLSPEC int SDLCALL SDL_memcmp(const void *s1, const void *s2, size_t len);

#endif

#ifdef HAVE_STRLEN
#define SDL_strlen      strlen
#else

extern DECLSPEC size_t SDLCALL SDL_strlen(const char *string);

#endif

#ifdef HAVE_STRLCPY
#define SDL_strlcpy     strlcpy
#else

extern DECLSPEC size_t SDLCALL SDL_strlcpy(char *dst, const char *src, size_t maxlen);

#endif

#ifdef HAVE_STRLCAT
#define SDL_strlcat    strlcat
#else

extern DECLSPEC size_t SDLCALL SDL_strlcat(char *dst, const char *src, size_t maxlen);

#endif

#ifdef HAVE_STRDUP
#define SDL_strdup     strdup
#else

extern DECLSPEC char *SDLCALL SDL_strdup(const char *string);

#endif

#ifdef HAVE__STRREV
#define SDL_strrev      _strrev
#else

extern DECLSPEC char *SDLCALL SDL_strrev(char *string);

#endif

#ifdef HAVE__STRUPR
#define SDL_strupr      _strupr
#else

extern DECLSPEC char *SDLCALL SDL_strupr(char *string);

#endif

#ifdef HAVE__STRLWR
#define SDL_strlwr      _strlwr
#else

extern DECLSPEC char *SDLCALL SDL_strlwr(char *string);

#endif

#ifdef HAVE_STRCHR
#define SDL_strchr      strchr
#elif defined(HAVE_INDEX)
#define SDL_strchr      index
#else

extern DECLSPEC char *SDLCALL SDL_strchr(const char *string, int c);

#endif

#ifdef HAVE_STRRCHR
#define SDL_strrchr     strrchr
#elif defined(HAVE_RINDEX)
#define SDL_strrchr     rindex
#else

extern DECLSPEC char *SDLCALL SDL_strrchr(const char *string, int c);

#endif

#ifdef HAVE_STRSTR
#define SDL_strstr      strstr
#else

extern DECLSPEC char *SDLCALL SDL_strstr(const char *haystack, const char *needle);

#endif

#ifdef HAVE_ITOA
#define SDL_itoa        itoa
#else
#define SDL_itoa(value, string, radix)    SDL_ltoa((long)value, string, radix)
#endif

#ifdef HAVE__LTOA
#define SDL_ltoa        _ltoa
#else

extern DECLSPEC char *SDLCALL SDL_ltoa(long value, char *string, int radix);

#endif

#ifdef HAVE__UITOA
#define SDL_uitoa       _uitoa
#else
#define SDL_uitoa(value, string, radix)    SDL_ultoa((long)value, string, radix)
#endif

#ifdef HAVE__ULTOA
#define SDL_ultoa       _ultoa
#else

extern DECLSPEC char *SDLCALL SDL_ultoa(unsigned long value, char *string, int radix);

#endif

#ifdef HAVE_STRTOL
#define SDL_strtol      strtol
#else

extern DECLSPEC long SDLCALL SDL_strtol(const char *string, char **endp, int base);

#endif

#ifdef HAVE_STRTOUL
#define SDL_strtoul      strtoul
#else

extern DECLSPEC unsigned long SDLCALL SDL_strtoul(const char *string, char **endp, int base);

#endif

#ifdef SDL_HAS_64BIT_TYPE

#ifdef HAVE__I64TOA
#define SDL_lltoa       _i64toa
#else

extern DECLSPEC char *SDLCALL SDL_lltoa(Sint64 value, char *string, int radix);

#endif

#ifdef HAVE__UI64TOA
#define SDL_ulltoa      _ui64toa
#else

extern DECLSPEC char *SDLCALL SDL_ulltoa(Uint64 value, char *string, int radix);

#endif

#ifdef HAVE__STRTOI64
#define SDL_strtoll    _strtoi64
#elif defined(HAVE_STRTOLL)
#define SDL_strtoll     strtoll
#else
extern DECLSPEC Sint64 SDLCALL SDL_strtoll(const char *string, char **endp, int base);
#endif

#ifdef HAVE__STRTOUI64
#define SDL_strtoull    _strtoui64
#elif defined(HAVE_STRTOULL)
#define SDL_strtoull     strtoull
#else
extern DECLSPEC Uint64 SDLCALL SDL_strtoull(const char *string, char **endp, int base);
#endif

#endif /* SDL_HAS_64BIT_TYPE */

#ifdef HAVE_STRTOD
#define SDL_strtod      strtod
#else

extern DECLSPEC double SDLCALL SDL_strtod(const char *string, char **endp);

#endif

#ifdef HAVE_ATOI
#define SDL_atoi        atoi
#else
#define SDL_atoi(X)     SDL_strtol(X, NULL, 0)
#endif

#ifdef HAVE_ATOF
#define SDL_atof        atof
#else
#define SDL_atof(X)     SDL_strtod(X, NULL)
#endif

#ifdef HAVE_STRCMP
#define SDL_strcmp      strcmp
#else

extern DECLSPEC int SDLCALL SDL_strcmp(const char *str1, const char *str2);

#endif

#ifdef HAVE_STRNCMP
#define SDL_strncmp     strncmp
#else

extern DECLSPEC int SDLCALL SDL_strncmp(const char *str1, const char *str2, size_t maxlen);

#endif

#if defined(HAVE__STRICMP)
#define SDL_strcasecmp  _stricmp
#elif defined(HAVE_STRCASECMP)
#define SDL_strcasecmp  strcasecmp
#else

extern DECLSPEC int SDLCALL SDL_strcasecmp(const char *str1, const char *str2);

#endif

#if defined(HAVE__STRNICMP)
#define SDL_strncasecmp _strnicmp
#elif defined(HAVE_STRNCASECMP)
#define SDL_strncasecmp strncasecmp
#else

extern DECLSPEC int SDLCALL SDL_strncasecmp(const char *str1, const char *str2, size_t maxlen);

#endif

#ifdef HAVE_SSCANF
#define SDL_sscanf      sscanf
#else

extern DECLSPEC int SDLCALL SDL_sscanf(const char *text, const char *fmt, ...);

#endif

#if defined(HAVE_SNPRINTF) && !(defined(__WATCOMC__) || defined(_WIN32))
#define SDL_snprintf    snprintf
#else

extern DECLSPEC int SDLCALL SDL_snprintf(char *text, size_t maxlen, const char *fmt, ...);

#endif

#if defined(HAVE_VSNPRINTF) && !(defined(__WATCOMC__) || defined(_WIN32))
#define SDL_vsnprintf   vsnprintf
#else

extern DECLSPEC int SDLCALL SDL_vsnprintf(char *text, size_t maxlen, const char *fmt, va_list ap);

#endif

#ifndef HAVE_M_PI
#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288   /**< pi */
#endif
#endif

extern DECLSPEC double SDLCALL SDL_acos(double x);
extern DECLSPEC float SDLCALL SDL_acosf(float x);
extern DECLSPEC double SDLCALL SDL_asin(double x);
extern DECLSPEC float SDLCALL SDL_asinf(float x);
extern DECLSPEC double SDLCALL SDL_atan(double x);
extern DECLSPEC float SDLCALL SDL_atanf(float x);
extern DECLSPEC double SDLCALL SDL_atan2(double x, double y);
extern DECLSPEC float SDLCALL SDL_atan2f(float x, float y);
extern DECLSPEC double SDLCALL SDL_ceil(double x);
extern DECLSPEC float SDLCALL SDL_ceilf(float x);
extern DECLSPEC double SDLCALL SDL_copysign(double x, double y);
extern DECLSPEC float SDLCALL SDL_copysignf(float x, float y);
extern DECLSPEC double SDLCALL SDL_cos(double x);
extern DECLSPEC float SDLCALL SDL_cosf(float x);
extern DECLSPEC double SDLCALL SDL_exp(double x);
extern DECLSPEC float SDLCALL SDL_expf(float x);
extern DECLSPEC double SDLCALL SDL_fabs(double x);
extern DECLSPEC float SDLCALL SDL_fabsf(float x);
extern DECLSPEC double SDLCALL SDL_floor(double x);
extern DECLSPEC float SDLCALL SDL_floorf(float x);
extern DECLSPEC double SDLCALL SDL_trunc(double x);
extern DECLSPEC float SDLCALL SDL_truncf(float x);
extern DECLSPEC double SDLCALL SDL_fmod(double x, double y);
extern DECLSPEC float SDLCALL SDL_fmodf(float x, float y);
extern DECLSPEC double SDLCALL SDL_log(double x);
extern DECLSPEC float SDLCALL SDL_logf(float x);
extern DECLSPEC double SDLCALL SDL_log10(double x);
extern DECLSPEC float SDLCALL SDL_log10f(float x);
extern DECLSPEC double SDLCALL SDL_pow(double x, double y);
extern DECLSPEC float SDLCALL SDL_powf(float x, float y);
extern DECLSPEC double SDLCALL SDL_scalbn(double x, int n);
extern DECLSPEC float SDLCALL SDL_scalbnf(float x, int n);
extern DECLSPEC double SDLCALL SDL_sin(double x);
extern DECLSPEC float SDLCALL SDL_sinf(float x);
extern DECLSPEC double SDLCALL SDL_sqrt(double x);
extern DECLSPEC float SDLCALL SDL_sqrtf(float x);
extern DECLSPEC double SDLCALL SDL_tan(double x);
extern DECLSPEC float SDLCALL SDL_tanf(float x);

/**
 * @name SDL_ICONV Error Codes
 * The SDL implementation of iconv() returns these error codes
 */
/*@{*/
#define SDL_ICONV_ERROR  (size_t)-1
#define SDL_ICONV_E2BIG  (size_t)-2
#define SDL_ICONV_EILSEQ (size_t)-3
#define SDL_ICONV_EINVAL (size_t)-4

/*@}*/

#if defined(HAVE_ICONV) && defined(HAVE_ICONV_H)
#define SDL_iconv_t     iconv_t
#define SDL_iconv_open  iconv_open
#define SDL_iconv_close iconv_close
#else
typedef struct _SDL_iconv_t *SDL_iconv_t;

extern DECLSPEC SDL_iconv_t SDLCALL SDL_iconv_open(const char *tocode, const char *fromcode);

extern DECLSPEC int SDLCALL SDL_iconv_close(SDL_iconv_t cd);

#endif

extern DECLSPEC size_t SDLCALL SDL_iconv(SDL_iconv_t cd, const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);

/** This function converts a string between encodings in one pass, returning a
 *  string that must be freed with SDL_free() or NULL on error.
 */
extern DECLSPEC char *SDLCALL SDL_iconv_string(const char *tocode, const char *fromcode, const char *inbuf, size_t inbytesleft);

#define SDL_iconv_utf8_locale(S)    SDL_iconv_string("", "UTF-8", S, SDL_strlen(S)+1)
#define SDL_iconv_utf8_ucs2(S)        (Uint16 *)SDL_iconv_string("UCS-2", "UTF-8", S, SDL_strlen(S)+1)
#define SDL_iconv_utf8_ucs4(S)        (Uint32 *)SDL_iconv_string("UCS-4", "UTF-8", S, SDL_strlen(S)+1)

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_stdinc_h_ */
