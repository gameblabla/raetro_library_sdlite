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
 * Copyright (c) 1997-2020 Sam Lantinga <slouken@libsdl.org>
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

//#include "SDL_assert.h"
#include "SDL_poll.h"

#ifdef HAVE_POLL

#include <poll.h>

#else
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <errno.h>

int SDL_IOReady(int fd, SDL_bool forWrite, int timeoutMS) {
	int result;

	/* Note: We don't bother to account for elapsed time if we get EINTR */
	do {
#ifdef HAVE_POLL
		struct pollfd info;

		info.fd = fd;
		if(forWrite) {
			info.events = POLLOUT;
		} else {
			info.events = POLLIN | POLLPRI;
		}
		result = poll(&info, 1, timeoutMS);
#else
		fd_set rfdset, *rfdp = NULL;
		fd_set wfdset, *wfdp = NULL;
		struct timeval tv, *tvp = NULL;

		/* If this assert triggers we'll corrupt memory here */
		SDL_assert(fd >= 0 && fd < FD_SETSIZE);

		if (forWrite) {
			FD_ZERO(&wfdset);
			FD_SET(fd, &wfdset);
			wfdp = &wfdset;
		} else {
			FD_ZERO(&rfdset);
			FD_SET(fd, &rfdset);
			rfdp = &rfdset;
		}

		if (timeoutMS >= 0) {
			tv.tv_sec = timeoutMS / 1000;
			tv.tv_usec = (timeoutMS % 1000) * 1000;
			tvp = &tv;
		}

		result = select(fd + 1, rfdp, wfdp, NULL, tvp);
#endif /* HAVE_POLL */

	} while (result < 0 && errno == EINTR);

	return result;
}
