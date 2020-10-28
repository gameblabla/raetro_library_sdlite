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
 * @file SDL_cpuinfo.h
 * @brief CPU Feature Detection routines.
 ******************************************************************************/
/*!*****************************************************************************
 * @defgroup cpu CPU Info
 * @brief CPU Feature Detection
 * @details This category contains functions for gathering information about
 * the processor's features.
 * @{
 ******************************************************************************/
#ifndef SDL_cpuinfo_h_
#define SDL_cpuinfo_h_

#include "SDL_stdinc.h"

#if !defined(SDL_DISABLE_ARM_NEON_H)
#  if defined(_ARM_NEON_)
#    include <arm_neon.h>
#  endif
#endif

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Guess the cacheline size
 *
 * This is a guess for the cacheline size used for padding.
 * Most x86 processors have a 64 byte cache line.
 * The 64-bit PowerPC processors have a 128 byte cache line.
 * We'll use the larger value to be generally safe.
 */
#define SDL_CACHELINE_SIZE  128

/**
 * @brief Get the number of CPU cores available.
 *
 *  This function returns the number of CPU cores available.
 */
extern DECLSPEC int SDLCALL SDL_GetCPUCount(void);

/**
 * @brief Get L1 cache line size of the CPU
 *
 * This is useful for determining multi-threaded structure padding
 * or SIMD prefetch sizes.
 */
extern DECLSPEC int SDLCALL SDL_GetCPUCacheLineSize(void);

/**
 * @brief Check if CPU has ARM SIMD (ARMv6) features.
 *
 * This function returns true if the CPU has ARM SIMD (ARMv6) features.
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasARMSIMD(void);

/**
 * @brief Check if the CPU has ARM NEON features.
 *
 * This function returns true if the CPU has NEON (ARM SIMD) features.
 */
extern DECLSPEC SDL_bool SDLCALL SDL_HasARMNEON(void);

/**
 * @brief Get the amount of RAM in MB.
 *
 * This function returns the amount of RAM configured in the system, in MB.
 */
extern DECLSPEC int SDLCALL SDL_GetSystemRAM(void);

/**
 * @brief Report the alignment this system needs for SIMD allocations.
 *
 * This will return the minimum number of bytes to which a pointer must be
 * aligned to be compatible with SIMD instructions on the current machine.
 * For example, if the machine supports SSE only, it will return 16, but if
 * it supports AVX-512F, it'll return 64 (etc). This only reports values for
 * instruction sets SDL knows about, so if your SDL build doesn't have
 * SDL_HasAVX512F(), then it might return 16 for the SSE support it sees and
 * not 64 for the AVX-512 instructions that exist but SDL doesn't know about.
 * Plan accordingly.
 */
extern DECLSPEC size_t SDLCALL SDL_SIMDGetAlignment(void);

/**
 * @brief Allocate memory in a SIMD-friendly way.
 *
 * This will allocate a block of memory that is suitable for use with SIMD
 * instructions. Specifically, it will be properly aligned and padded for
 * the system's supported vector instructions.
 *
 * The memory returned will be padded such that it is safe to read or write
 * an incomplete vector at the end of the memory block. This can be useful
 * so you don't have to drop back to a scalar fallback at the end of your
 * SIMD processing loop to deal with the final elements without overflowing
 * the allocated buffer.
 *
 * You must free this memory with SDL_FreeSIMD(), not free() or SDL_free()
 * or delete[], etc.
 *
 * Note that SDL will only deal with SIMD instruction sets it is aware of;
 * for example, SDL 2.0.8 knows that SSE wants 16-byte vectors
 * (SDL_HasSSE()), and AVX2 wants 32 bytes (SDL_HasAVX2()), but doesn't
 * know that AVX-512 wants 64. To be clear: if you can't decide to use an
 * instruction set with an SDL_Has*() function, don't use that instruction
 * set with memory allocated through here.
 *
 * SDL_AllocSIMD(0) will return a non-NULL pointer, assuming the system isn't
 *  out of memory.
 *
 * @param len The length, in bytes, of the block to allocated. The actual
 *             allocated block might be larger due to padding, etc.
 * @return Pointer to newly-allocated block, NULL if out of memory.
 *
 * @sa SDL_SIMDAlignment
 * @sa SDL_SIMDFree
 */
extern DECLSPEC void *SDLCALL SDL_SIMDAlloc(const size_t len);

/**
 * @brief Deallocate memory obtained from SDL_SIMDAlloc
 *
 * It is not valid to use this function on a pointer from anything but
 *  SDL_SIMDAlloc(). It can't be used on pointers from malloc, realloc,
 *  SDL_malloc, memalign, new[], etc.
 *
 * However, SDL_SIMDFree(NULL) is a legal no-op.
 *
 * @sa SDL_SIMDAlloc
 */
extern DECLSPEC void SDLCALL SDL_SIMDFree(void *ptr);

/*!*****************************************************************************
 * @memberof deprecated
 * @{
 ******************************************************************************/
/** @deprecated [function] SDL_HasRDTSC has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasRDTSC(void);

/** @deprecated [function] SDL_HasMMX has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasMMX(void);

/** @deprecated [function] SDL_HasMMXExt has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasMMXExt(void);

/** @deprecated [function] SDL_Has3DNow has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_Has3DNow(void);

/** @deprecated [function] SDL_Has3DNowExt has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_Has3DNowExt(void);

/** @deprecated [function] SDL_HasSSE has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE(void);

/** @deprecated [function] SDL_HasSSE2 has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE2(void);

/** @deprecated [function] SDL_HasSSE2 has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE3(void);

/** @deprecated [function] SDL_HasSSE2 has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE41(void);

/** @deprecated [function] SDL_HasSSE2 has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE42(void);

/** @deprecated [function] SDL_HasSSE2 has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasAVX(void);

/** @deprecated [function] SDL_HasSSE2 has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasAVX2(void);

/** @deprecated [function] SDL_HasSSE2 has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasAVX512F(void);

/** @deprecated [function] SDL_HasAltiVec has been deprecated */
extern DECLSPEC SDL_bool SDLCALL SDL_HasAltiVec(void);
/*!@}*/

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_cpuinfo_h_ */
