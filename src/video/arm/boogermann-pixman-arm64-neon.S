/*
 * Copyright © 2009 Nokia Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Author:  Siarhei Siamashka (siarhei.siamashka@nokia.com)
 */

/* Prevent the stack from becoming executable for no reason... */
#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif

.text
.arch armv8-a

.altmacro
.p2align 2

#include "pixman-arm-asm.h"
#include "pixman-arma64-neon-asm.h"

/* Global configuration options and preferences */

/*
 * The code can optionally make use of unaligned memory accesses to improve
 * performance of handling leading/trailing pixels for each scanline.
 * Configuration variable RESPECT_STRICT_ALIGNMENT can be set to 0 for
 * example in linux if unaligned memory accesses are not configured to
 * generate.exceptions.
 */
.set RESPECT_STRICT_ALIGNMENT, 1

/*
 * Set default prefetch type. There is a choice between the following options:
 *
 * PREFETCH_TYPE_NONE (may be useful for the ARM cores where PLD is set to work
 * as NOP to workaround some HW bugs or for whatever other reason)
 *
 * PREFETCH_TYPE_SIMPLE (may be useful for simple single-issue ARM cores where
 * advanced prefetch intruduces heavy overhead)
 *
 * PREFETCH_TYPE_ADVANCED (useful for superscalar cores such as ARM Cortex-A8
 * which can run ARM and NEON instructions simultaneously so that extra ARM
 * instructions do not add (many) extra cycles, but improve prefetch efficiency)
 *
 * Note: some types of function can't support advanced prefetch and fallback
 *       to simple one (those which handle 24bpp pixels)
 */
.set PREFETCH_TYPE_DEFAULT, PREFETCH_TYPE_ADVANCED

/* Prefetch distance in pixels for simple prefetch */
.set PREFETCH_DISTANCE_SIMPLE, 64

.macro RGBtoRGBPixelAlpha_process_pixblock_head
    umull       v8.8h, v3.8b, v0.8b
    umull       v9.8h, v3.8b, v1.8b
    umull       v10.8h, v3.8b, v2.8b
.endm

.macro RGBtoRGBPixelAlpha_process_pixblock_tail
    urshr       v11.8h, v8.8h, #8
    mov         v30.8b, v31.8b
    mov         v31.8b, v3.8b
    mov         v3.8b, v30.8b
    urshr       v12.8h, v9.8h, #8
    urshr       v13.8h, v10.8h, #8
    raddhn      v30.8b, v11.8h, v8.8h
    raddhn      v29.8b, v12.8h, v9.8h
    raddhn      v28.8b, v13.8h, v10.8h
.endm

.macro RGBtoRGBPixelAlpha_process_pixblock_tail_head
        urshr       v11.8h, v8.8h, #8
        mov         v30.8b, v31.8b
        mov         v31.8b, v3.8b
        mov         v3.8b, v31.8b
        urshr       v12.8h, v9.8h, #8
        urshr       v13.8h, v10.8h, #8
    fetch_src_pixblock
        raddhn      v30.8b, v11.8h, v8.8h
                                    PF add PF_X, PF_X, #8
                                    PF tst PF_CTL, #0xF
                                    PF beq 10f
                                    PF add PF_X, PF_X, #8
                                    PF sub PF_CTL, PF_CTL, #1
10:
        raddhn      v29.8b, v12.8h, v9.8h
        raddhn      v28.8b, v13.8h, v10.8h
    umull       v8.8h, v3.8b, v0.8b
    umull       v9.8h, v3.8b, v1.8b
    umull       v10.8h, v3.8b, v2.8b
         st4    {v28.8b, v29.8b, v30.8b, v31.8b}, [DST_W], #32
                                    PF cmp PF_X, ORIG_W
                                    PF lsl DUMMY, PF_X, src_bpp_shift
                                    PF prfm PREFETCH_MODE, [PF_SRC, DUMMY]
                                    PF ble 10f
                                    PF sub PF_X, PF_X, ORIG_W
                                    PF subs PF_CTL, PF_CTL, #0x10
                                    PF ble 10f
                                    PF lsl DUMMY, SRC_STRIDE, #src_bpp_shift
                                    PF ldrsb DUMMY, [PF_SRC, DUMMY]
                                    PF add PF_SRC, PF_SRC, #1
10:
.endm

generate_composite_function \
    BlitRGBtoRGBPixelAlphaARMNEONAsm, 32, 0, 32, \
    FLAG_DST_WRITEONLY | FLAG_DEINTERLEAVE_32BPP, \
    8, /* number of pixels, processed in a single block */ \
    10, /* prefetch distance */ \
    default_init, \
    default_cleanup, \
    RGBtoRGBPixelAlpha_process_pixblock_head, \
    RGBtoRGBPixelAlpha_process_pixblock_tail, \
    RGBtoRGBPixelAlpha_process_pixblock_tail_head, \
    28, /* dst_w_basereg */ \
    0, /* dst_r_basereg */ \
    0, /* src_basereg   */ \
    0  /* mask_basereg  */

/******************************************************************************/

.macro ARGBto565PixelAlpha_process_pixblock_head
    ushll       v8.8h,  v1.8b,  #7
    sli         v8.8h,  v8.8h,  #1
    ushll       v14.8h, v2.8b,  #7
    sli         v14.8h, v14.8h, #1
    ushll       v9.8h,  v0.8b,  #7
    sli         v9.8h,  v9.8h,  #1
.endm

.macro ARGBto565PixelAlpha_process_pixblock_tail
    sri         v14.8h, v8.8h, #5
    sri         v14.8h, v9.8h, #11
    mov         v28.d[0], v14.d[0]
    mov         v29.d[0], v14.d[1]
.endm

.macro ARGBto565PixelAlpha_process_pixblock_tail_head
    sri         v14.8h, v8.8h, #5
                                PF add PF_X, PF_X, #8
                                PF tst PF_CTL, #0xF
    fetch_src_pixblock
                                PF beq 10f
                                PF add PF_X, PF_X, #8
                                PF sub PF_CTL, PF_CTL, #1
10:
    sri         v14.8h, v9.8h, #11
    mov         v28.d[0], v14.d[0]
    mov         v29.d[0], v14.d[1]
                                PF cmp PF_X, ORIG_W
                                PF lsl DUMMY, PF_X, #src_bpp_shift
                                PF prfm PREFETCH_MODE, [PF_SRC, DUMMY]
    ushll       v8.8h, v1.8b, #7
    sli         v8.8h, v8.8h, #1
    st1        {v14.8h}, [DST_W], #16
                                PF ble 10f
                                PF sub PF_X, PF_X, ORIG_W
                                PF subs PF_CTL, PF_CTL, #0x10
10:
    ushll       v14.8h, v2.8b, #7
    sli         v14.8h, v14.8h, #1
                                PF ble 10f
                                PF lsl DUMMY, SRC_STRIDE, #src_bpp_shift
                                PF ldrsb DUMMY, [PF_SRC, DUMMY]
                                PF add PF_SRC, PF_SRC, #1
10:
    ushll       v9.8h, v0.8b, #7
    sli         v9.8h, v9.8h, #1
.endm

generate_composite_function \
    BlitARGBto565PixelAlphaARMNEONAsm, 32, 0, 16, \
    FLAG_DST_WRITEONLY | FLAG_DEINTERLEAVE_32BPP, \
    8, /* number of pixels, processed in a single block */ \
    10, /* prefetch distance */ \
    default_init, \
    default_cleanup, \
    ARGBto565PixelAlpha_process_pixblock_head, \
    ARGBto565PixelAlpha_process_pixblock_tail, \
    ARGBto565PixelAlpha_process_pixblock_tail_head

/******************************************************************************/
