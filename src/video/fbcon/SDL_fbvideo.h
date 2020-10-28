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

#include "SDL_config.h"

#ifndef SDL_fbvideo_h_
#define SDL_fbvideo_h_

#include <sys/types.h>
#include <termios.h>
#include <linux/fb.h>

#include "SDL_mouse.h"
#include "SDL_mutex.h"
#include "SDL_thread.h"
#include "../SDL_sysvideo.h"

#if SDL_INPUT_TSLIB
#include "tslib.h"
#endif

/* Hidden "this" pointer for the video functions */
#define _THIS    SDL_VideoDevice *this

typedef void FB_bitBlit(Uint8 *src_pos, int src_right_delta, /* pixels, not bytes */
						int src_down_delta,  /* pixels, not bytes */
						Uint8 *dst_pos, int dst_linebytes, int width, int height);

/* This is the structure we use to keep track of video memory */
typedef struct vidmem_bucket {
	struct vidmem_bucket *prev;
	int used;
	int dirty;
	char *base;
	unsigned int size;
	struct vidmem_bucket *next;
} vidmem_bucket;

/* Private display data */
struct SDL_PrivateVideoData {
	int console_fd;
	struct fb_var_screeninfo cache_vinfo;
	struct fb_var_screeninfo saved_vinfo;
	int saved_cmaplen;
	__u16 *saved_cmap;

	int current_vt;
	int saved_vt;
	int keyboard_fd;
	int saved_kbd_mode;
	struct termios saved_kbd_termios;

	int mouse_fd;
#if SDL_INPUT_TSLIB
	struct tsdev *ts_dev;
#endif
	char *mapped_mem;
	char *shadow_mem;
	int mapped_memlen;
	int mapped_offset;
	char *mapped_io;
	long mapped_iolen;
	int flip_page;
	char *flip_address[3];
#if !SDL_THREADS_DISABLED
	int current_page;
	int new_page;
	SDL_mutex *triplebuf_mutex;
	SDL_cond *triplebuf_cond;
	SDL_Thread *triplebuf_thread;
	int triplebuf_thread_stop;
#endif
	int rotate;
	int shadow_fb;          /* Tells whether a shadow is being used. */
	FB_bitBlit *blitFunc;
	int physlinebytes;      /* Length of a line in bytes in physical fb */

#define NUM_MODELISTS    4  /* 8, 16, 24, and 32 bits-per-pixel */
	int SDL_nummodes[NUM_MODELISTS];
	SDL_Rect **SDL_modelist[NUM_MODELISTS];

	vidmem_bucket surfaces;
	int surfaces_memtotal;
	int surfaces_memleft;

	SDL_mutex *hw_lock;
	int switched_away;
	struct fb_var_screeninfo screen_vinfo;
	Uint32 screen_arealen;
	Uint8 *screen_contents;
	__u16 screen_palette[3 * 256];

	void (*wait_vbl)(_THIS);

	void (*wait_idle)(_THIS);
};
/* Old variable names */
#define console_fd            (this->hidden->console_fd)
#define current_vt            (this->hidden->current_vt)
#define saved_vt              (this->hidden->saved_vt)
#define keyboard_fd           (this->hidden->keyboard_fd)
#define saved_kbd_mode        (this->hidden->saved_kbd_mode)
#define saved_kbd_termios     (this->hidden->saved_kbd_termios)
#define mouse_fd              (this->hidden->mouse_fd)
#if SDL_INPUT_TSLIB
#define ts_dev				  (this->hidden->ts_dev)
#endif
#define cache_vinfo           (this->hidden->cache_vinfo)
#define saved_vinfo           (this->hidden->saved_vinfo)
#define saved_cmaplen         (this->hidden->saved_cmaplen)
#define saved_cmap            (this->hidden->saved_cmap)
#define mapped_mem            (this->hidden->mapped_mem)
#define shadow_mem            (this->hidden->shadow_mem)
#define mapped_memlen         (this->hidden->mapped_memlen)
#define mapped_offset         (this->hidden->mapped_offset)
#define mapped_io             (this->hidden->mapped_io)
#define mapped_iolen          (this->hidden->mapped_iolen)
#define flip_page             (this->hidden->flip_page)
#define flip_address          (this->hidden->flip_address)
#if !SDL_THREADS_DISABLED
#define current_page          (this->hidden->current_page)
#define new_page              (this->hidden->new_page)
#define triplebuf_mutex       (this->hidden->triplebuf_mutex)
#define triplebuf_cond        (this->hidden->triplebuf_cond)
#define triplebuf_thread      (this->hidden->triplebuf_thread)
#define triplebuf_thread_stop (this->hidden->triplebuf_thread_stop)
#endif
#define rotate                (this->hidden->rotate)
#define shadow_fb             (this->hidden->shadow_fb)
#define blitFunc              (this->hidden->blitFunc)
#define physlinebytes         (this->hidden->physlinebytes)
#define SDL_nummodes          (this->hidden->SDL_nummodes)
#define SDL_modelist          (this->hidden->SDL_modelist)
#define surfaces              (this->hidden->surfaces)
#define surfaces_memtotal     (this->hidden->surfaces_memtotal)
#define surfaces_memleft      (this->hidden->surfaces_memleft)
#define hw_lock               (this->hidden->hw_lock)
#define switched_away         (this->hidden->switched_away)
#define screen_vinfo          (this->hidden->screen_vinfo)
#define screen_arealen        (this->hidden->screen_arealen)
#define screen_contents       (this->hidden->screen_contents)
#define screen_palette        (this->hidden->screen_palette)
#define wait_vbl              (this->hidden->wait_vbl)
#define wait_idle             (this->hidden->wait_idle)

/* These functions are defined in SDL_fbvideo.c */
extern void FB_SavePaletteTo(_THIS, int palette_len, __u16 *area);

extern void FB_RestorePaletteFrom(_THIS, int palette_len, __u16 *area);

/* These are utility functions for working with video surfaces */
static __inline__ void FB_AddBusySurface(SDL_Surface *surface) {
	((vidmem_bucket *) surface->hwdata)->dirty = 1;
}

static __inline__ int FB_IsSurfaceBusy(SDL_Surface *surface) {
	return ((vidmem_bucket *) surface->hwdata)->dirty;
}

static __inline__ void FB_WaitBusySurfaces(_THIS) {
	vidmem_bucket *bucket;

	/* Wait for graphic operations to complete */
	wait_idle(this);

	/* Clear all surface dirty bits */
	for (bucket = &surfaces; bucket; bucket = bucket->next) {
		bucket->dirty = 0;
	}
}

static __inline__ void FB_dst_to_xy(_THIS, SDL_Surface *dst, int *x, int *y) {
	*x = (long) ((char *) dst->pixels - mapped_mem) % this->screen->pitch;
	*y = (long) ((char *) dst->pixels - mapped_mem) / this->screen->pitch;
	if(dst == this->screen) {
		*x += this->offset_x;
		*y += this->offset_y;
	}
}

#endif /* SDL_fbvideo_h_ */
