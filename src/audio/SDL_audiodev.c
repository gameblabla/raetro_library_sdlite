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

/* Get the name of the audio device we use for output */

#if SDL_AUDIO_DRIVER_OSS

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "SDL_stdinc.h"
#include "SDL_audiodev_c.h"

#ifndef _PATH_DEV_DSP
#define _PATH_DEV_DSP  "/dev/dsp"
#endif

#ifndef _PATH_DEV_DSP24
#define _PATH_DEV_DSP24    "/dev/sound/dsp"
#endif
#ifndef _PATH_DEV_AUDIO
#define _PATH_DEV_AUDIO    "/dev/audio"
#endif


int SDL_OpenAudioPath(char *path, int maxlen, int flags, int classic) {
	const char *audiodev;
	int audio_fd;
	char audiopath[1024];

	/* Figure out what our audio device is */
	if(((audiodev = SDL_getenv("SDL_PATH_DSP")) == NULL) && ((audiodev = SDL_getenv("AUDIODEV")) == NULL)) {
		if(classic) {
			audiodev = _PATH_DEV_AUDIO;
		} else {
			struct stat sb;

			/* Added support for /dev/sound/\* in Linux 2.4 */
			if(((stat("/dev/sound", &sb) == 0) && S_ISDIR(sb.st_mode)) && ((stat(_PATH_DEV_DSP24, &sb) == 0) && S_ISCHR(sb.st_mode))) {
				audiodev = _PATH_DEV_DSP24;
			} else {
				audiodev = _PATH_DEV_DSP;
			}
		}
	}
	audio_fd = open(audiodev, flags, 0);

	/* If the first open fails, look for other devices */
	if((audio_fd < 0) && (SDL_strlen(audiodev) < (sizeof(audiopath) - 3))) {
		int exists, instance;
		struct stat sb;

		instance = 1;
		do { /* Don't use errno ENOENT - it may not be thread-safe */
			SDL_snprintf(audiopath, SDL_arraysize(audiopath), "%s%d", audiodev, instance++);
			exists = 0;
			if(stat(audiopath, &sb) == 0) {
				exists = 1;
				audio_fd = open(audiopath, flags, 0);
			}
		} while (exists && (audio_fd < 0));
		audiodev = audiopath;
	}
	if(path != NULL) {
		SDL_strlcpy(path, audiodev, maxlen);
		path[maxlen - 1] = '\0';
	}
	return (audio_fd);
}

#endif /* Audio driver selection */
