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

/* Resize event handling code for SDL */

#include "SDL_events.h"
#include "SDL_events_c.h"
#include "../video/SDL_sysvideo.h"


/* Keep the last resize event so we don't post duplicates */
static struct {
	int w;
	int h;
} last_resize;

/* This is global for SDL_eventloop.c */
int SDL_PrivateResize(int w, int h) {
	int posted;
	SDL_Event events[32];

	/* See if this event would change the video surface */
	if(!w || !h || ((last_resize.w == w) && (last_resize.h == h)) || !SDL_VideoSurface) {
		return (0);
	}
	last_resize.w = w;
	last_resize.h = h;

	SDL_SetMouseRange(w, h);

	/* Pull out all old resize events */
	SDL_PeepEvents(events, sizeof(events) / sizeof(events[0]), SDL_GETEVENT, SDL_VIDEORESIZEMASK);

	/* Post the event, if desired */
	posted = 0;
	if(SDL_ProcessEvents[SDL_VIDEORESIZE] == SDL_ENABLE) {
		SDL_Event event;
		event.type = SDL_VIDEORESIZE;
		event.resize.w = w;
		event.resize.h = h;
		if((SDL_EventOK == NULL) || (*SDL_EventOK)(&event)) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}
	return (posted);
}
