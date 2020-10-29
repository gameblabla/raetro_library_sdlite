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
/* Initialization code for SDL */

#include "SDL.h"

#include "SDL_fatal.h"
#include "SDL_revision.h"
#include <unistd.h>

#if !SDL_VIDEO_DISABLED

#include "video/SDL_leaks.h"

#endif

#if SDL_THREAD_PTH
#include <pth.h>
#endif

/* Initialization/Cleanup routines */
#if !SDL_JOYSTICK_DISABLED

extern int SDL_JoystickInit(void);

extern void SDL_JoystickQuit(void);

#endif
#if !SDL_CDROM_DISABLED

extern int SDL_CDROMInit(void);

extern void SDL_CDROMQuit(void);

#endif
#if !SDL_TIMERS_DISABLED

extern void SDL_StartTicks(void);

extern int SDL_TimerInit(void);

extern void SDL_TimerQuit(void);

#endif

/* This is not declared in any header, although it is shared between some
    parts of SDL, because we don't want anything calling it without an
    extremely good reason. */
extern SDL_NORETURN void SDL_ExitProcess(int exitcode);

SDL_NORETURN void SDL_ExitProcess(int exitcode) {
#if defined(HAVE__EXIT) /* Upper case _Exit() */
	_Exit(exitcode);
#else
	_exit(exitcode);
#endif
}

/* The current SDL version */
static SDL_version version = {
	SDL_MAJOR_VERSION,
	SDL_MINOR_VERSION,
	SDL_PATCHLEVEL
};

/* The initialized subsystems */
static Uint32 SDL_initialized = 0;
#if !SDL_TIMERS_DISABLED
static Uint32 ticks_started = 0;
#endif

#ifdef CHECK_LEAKS
int surfaces_allocated = 0;
#endif

int SDL_InitSubSystem(Uint32 flags) {
#if !SDL_TIMERS_DISABLED
	/* Initialize the timer subsystem */
	if(!ticks_started) {
		SDL_StartTicks();
		ticks_started = 1;
	}
	if((flags & SDL_INIT_TIMER) && !(SDL_initialized & SDL_INIT_TIMER)) {
		if(SDL_TimerInit() < 0) {
			return (-1);
		}
		SDL_initialized |= SDL_INIT_TIMER;
	}
#else
	if(flags & SDL_INIT_TIMER) {
		SDL_SetError("SDL not built with timer support");
		return (-1);
	}
#endif

#if !SDL_VIDEO_DISABLED
	/* Initialize the video/event subsystem */
	if((flags & SDL_INIT_VIDEO) && !(SDL_initialized & SDL_INIT_VIDEO)) {
		if(SDL_VideoInit(SDL_getenv("SDL_VIDEODRIVER"), (flags & SDL_INIT_EVENTTHREAD)) < 0) {
			return (-1);
		}
		SDL_initialized |= SDL_INIT_VIDEO;
	}
#else
	if ( flags & SDL_INIT_VIDEO ) {
		SDL_SetError("SDL not built with video support");
		return(-1);
	}
#endif

#if !SDL_AUDIO_DISABLED
	/* Initialize the audio subsystem */
	if((flags & SDL_INIT_AUDIO) && !(SDL_initialized & SDL_INIT_AUDIO)) {
		if(SDL_AudioInit(SDL_getenv("SDL_AUDIODRIVER")) < 0) {
			return (-1);
		}
		SDL_initialized |= SDL_INIT_AUDIO;
	}
#else
	if ( flags & SDL_INIT_AUDIO ) {
		SDL_SetError("SDL not built with audio support");
		return(-1);
	}
#endif

#if !SDL_JOYSTICK_DISABLED
	/* Initialize the joystick subsystem */
	if((flags & SDL_INIT_JOYSTICK) && !(SDL_initialized & SDL_INIT_JOYSTICK)) {
		if(SDL_JoystickInit() < 0) {
			return (-1);
		}
		SDL_initialized |= SDL_INIT_JOYSTICK;
	}
#else
	if(flags & SDL_INIT_JOYSTICK) {
		SDL_SetError("SDL not built with joystick support");
		return (-1);
	}
#endif

#if !SDL_CDROM_DISABLED
	/* Initialize the CD-ROM subsystem */
	if((flags & SDL_INIT_CDROM) && !(SDL_initialized & SDL_INIT_CDROM)) {
		if(SDL_CDROMInit() < 0) {
			return (-1);
		}
		SDL_initialized |= SDL_INIT_CDROM;
	}
#else
	if(flags & SDL_INIT_CDROM) {
		SDL_SetError("SDL not built with cdrom support");
		return (-1);
	}
#endif
	return (0);
}

int SDL_Init(Uint32 flags) {
#if !SDL_THREADS_DISABLED && SDL_THREAD_PTH
	if (!pth_init()) {
		return -1;
	}
#endif

	/* Clear the error message */
	SDL_ClearError();

	/* Initialize the desired subsystems */
	if(SDL_InitSubSystem(flags) < 0) {
		return (-1);
	}

	/* Everything is initialized */
	if(!(flags & SDL_INIT_NOPARACHUTE)) {
		SDL_InstallParachute();
	}
	return (0);
}

void SDL_QuitSubSystem(Uint32 flags) {
	/* Shut down requested initialized subsystems */
#if !SDL_CDROM_DISABLED
	if((flags & SDL_initialized & SDL_INIT_CDROM)) {
		SDL_CDROMQuit();
		SDL_initialized &= ~SDL_INIT_CDROM;
	}
#endif
#if !SDL_JOYSTICK_DISABLED
	if((flags & SDL_initialized & SDL_INIT_JOYSTICK)) {
		SDL_JoystickQuit();
		SDL_initialized &= ~SDL_INIT_JOYSTICK;
	}
#endif
#if !SDL_AUDIO_DISABLED
	if((flags & SDL_initialized & SDL_INIT_AUDIO)) {
		SDL_AudioQuit();
		SDL_initialized &= ~SDL_INIT_AUDIO;
	}
#endif
#if !SDL_VIDEO_DISABLED
	if((flags & SDL_initialized & SDL_INIT_VIDEO)) {
		SDL_VideoQuit();
		SDL_initialized &= ~SDL_INIT_VIDEO;
	}
#endif
#if !SDL_TIMERS_DISABLED
	if((flags & SDL_initialized & SDL_INIT_TIMER)) {
		SDL_TimerQuit();
		SDL_initialized &= ~SDL_INIT_TIMER;
	}
#endif
}

Uint32 SDL_WasInit(Uint32 flags) {
	if(!flags) {
		flags = SDL_INIT_EVERYTHING;
	}
	return SDL_initialized & flags;
}

void SDL_Quit(void) {
	/* Quit all subsystems */
#ifdef DEBUG_BUILD
	printf("[SDL_Quit] : Enter! Calling QuitSubSystem()\n"); fflush(stdout);
#endif
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);

#ifdef CHECK_LEAKS
#ifdef DEBUG_BUILD
	printf("[SDL_Quit] : CHECK_LEAKS\n"); fflush(stdout);
#endif
	/* Print the number of surfaces not freed */
	if ( surfaces_allocated != 0 ) {
	  fprintf(stderr, "SDL Warning: %d SDL surfaces extant\n", surfaces_allocated);
	}
#endif
#ifdef DEBUG_BUILD
	printf("[SDL_Quit] : SDL_UninstallParachute()\n"); fflush(stdout);
#endif
	/* Uninstall any parachute signal handlers */
	SDL_UninstallParachute();

#if !SDL_THREADS_DISABLED && SDL_THREAD_PTH
	pth_kill();
#endif
#ifdef DEBUG_BUILD
	printf("[SDL_Quit] : Returning!\n"); fflush(stdout);
#endif

}

/* Return the library version number */
const SDL_version *SDL_Linked_Version(void) {
	return (&version);
}

/* Get the library version number */
void SDL_GetVersion(SDL_version *ver) {
	SDL_VERSION(ver);
}

/* Get the library source revision */
const char *SDL_GetRevision(void) {
	return SDL_REVISION;
}

/* Get the library source revision number */
int SDL_GetRevisionNumber(void) {
	return SDL_REVISION_NUMBER;
}

/* Get the name of the platform */
const char *SDL_GetPlatform() {
#if __LINUX__
	return "Linux";
#else
	return "Unknown (see SDL_platform.h)";
#endif
}
