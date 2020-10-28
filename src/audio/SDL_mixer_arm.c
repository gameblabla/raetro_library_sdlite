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
 * ARM Assembly Mix Routines:
 * Copyright (c) 2010 Palm, inc.
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
#include "SDL_stdinc.h"
#include "SDL_audio.h"

/* The volume ranges from 0 - 128 */
#define ADJUST_VOLUME(s, v) (s = (s*v)/SDL_MIX_MAXVOLUME)
#define ADJUST_VOLUME_U8(s, v)  (s = (((s-128)*v)/SDL_MIX_MAXVOLUME)+128)

#if defined(__ARM_NEON__)
void SDL_MixAudio_ARM_NEON_S16LSB(Uint8 *dst, const Uint8 *src, Uint32 len, int volume) {
	Sint16 src1, src2;
	int dst_sample;
	const int max_audioval = ((1 << (16 - 1)) - 1);
	const int min_audioval = -(1 << (16 - 1));

	len /= 2;

	if(((int) src & 1) == 0 && ((int) dst & 1) == 0) {

		// Align dst pointer
		for (; ((int) dst & 0xF) && len; --len) {
			src1 = ((src[1]) << 8 | src[0]);
			ADJUST_VOLUME(src1, volume);
			src2 = ((dst[1]) << 8 | dst[0]);
			src += 2;
			dst_sample = src1 + src2;
			if(dst_sample > max_audioval) {
				dst_sample = max_audioval;
			} else if(dst_sample < min_audioval) {
				dst_sample = min_audioval;
			}
			dst[0] = dst_sample & 0xFF;
			dst_sample >>= 8;
			dst[1] = dst_sample & 0xFF;
			dst += 2;
		}

		//printf( "Entering neon dst %p src %p len %d\n", dst, src, len);

		int Chunks = len >> 3;
		len &= 0x7;

		if(Chunks) {
			asm volatile (
			"vmov.i16   d0[0], %[Vol]                   \n\t"

			"L_%=:                                      \n\t" // generates a unique label

			"vldm       %[SrcP]!, { d2, d3 }            \n\t"
			"pld        [%[SrcP], #64]                  \n\t"

			"vldm       %[DstP],  { d10, d11 }          \n\t"
			"pld        [%[DstP], #64]                  \n\t"

			"vmull.s16  q2, d2, d0[0]                   \n\t" // multiply by volume and expand to 32 bits
			"vmull.s16  q3, d3, d0[0]                   \n\t" // multiply by volume and expand to 32 bits

			"vshrn.s32  d2, q2, #7                      \n\t"
			"vshrn.s32  d3, q3, #7                      \n\t"

			"vqadd.s16  q1, q1, q5                      \n\t"

			"subs       %[len], #1                      \n\t"
			"vstm       %[DstP]!, { d2, d3 }            \n\t"

			"bne        L_%=                            \n\t"

			: [DstP] "+r"(dst), [SrcP] "+r"(src), [len] "+r"(Chunks)
			: [Vol] "r"(volume)
			: "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "d10", "d11", "cc", "memory"
			);
		}
	}

	while (len--) {
		src1 = ((src[1]) << 8 | src[0]);
		ADJUST_VOLUME(src1, volume);
		src2 = ((dst[1]) << 8 | dst[0]);
		src += 2;
		dst_sample = src1 + src2;
		if(dst_sample > max_audioval) {
			dst_sample = max_audioval;
		} else if(dst_sample < min_audioval) {
			dst_sample = min_audioval;
		}
		dst[0] = dst_sample & 0xFF;
		dst_sample >>= 8;
		dst[1] = dst_sample & 0xFF;
		dst += 2;
	}
}

#endif

#if 0
void SDL_MixAudio_ARMv6_S16LSB(Uint8 *dst, const Uint8 *src, Uint32 len, int volume) {
	Sint16 src1, src2;
	int dst_sample;
	const int max_audioval = ((1 << (16 - 1)) - 1);
	const int min_audioval = -(1 << (16 - 1));

	len /= 2;

	// Does not appear to be any faster... needs further investigation.
	if(len && ((int) src & 1) == 0 && ((int) dst & 1) == 0) {
		asm volatile (
		"ldrsh      r6, [%[SrcP]], #2               \n\t"
		"ldrsh      r7, [%[DstP]]                   \n\t"

		"L_%=:                                      \n\t" // generates a unique label

		"mov        r4, r6                          \n\t"
		"pld        [%[SrcP], #64]                  \n\t"

		"mul        r4, r4, %[Vol]                  \n\t"
		"subs       %[len], #1                      \n\t"

		"mov        r5, r7                          \n\t"
		"pld        [%[DstP], #64]                  \n\t"

		"asr        r4, #7                          \n\t"
		"ldrnesh    r6, [%[SrcP]], #2               \n\t"

		"qadd16     r4, r4, r5                      \n\t"
		"ldrnesh    r7, [%[DstP], #2]               \n\t"

		"strh       r4, [%[DstP]], #2               \n\t"
		"bne        L_%=                            \n\t"

		: [DstP] "+r"(dst), [SrcP] "+r"(src), [len] "+r"(len)
		: [Vol] "r"(volume)
		: "r4", "r5", "r6", "r7", "cc", "memory"
		);
	}

	while (len--) {
		src1 = ((src[1]) << 8 | src[0]);
		ADJUST_VOLUME(src1, volume);
		src2 = ((dst[1]) << 8 | dst[0]);
		src += 2;
		dst_sample = src1 + src2;
		if(dst_sample > max_audioval) {
			dst_sample = max_audioval;
		} else if(dst_sample < min_audioval) {
			dst_sample = min_audioval;
		}
		dst[0] = dst_sample & 0xFF;
		dst_sample >>= 8;
		dst[1] = dst_sample & 0xFF;
		dst += 2;
	}
}

#endif

