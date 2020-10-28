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

#ifndef SDL_thread_c_h_
#define SDL_thread_c_h_

/* Need the definitions of SYS_ThreadHandle */
#if SDL_THREADS_DISABLED
#include "generic/SDL_systhread_c.h"
#elif SDL_THREAD_BEOS
#include "beos/SDL_systhread_c.h"
#elif SDL_THREAD_DC
#include "dc/SDL_systhread_c.h"
#elif SDL_THREAD_OS2
#include "os2/SDL_systhread_c.h"
#elif SDL_THREAD_PTH
#include "pth/SDL_systhread_c.h"
#elif SDL_THREAD_PTHREAD

#include "pthread/SDL_systhread_c.h"

#elif SDL_THREAD_SPROC
#include "irix/SDL_systhread_c.h"
#elif SDL_THREAD_WIN32
#include "win32/SDL_systhread_c.h"
#elif SDL_THREAD_SYMBIAN
#include "symbian/SDL_systhread_c.h"
#else
#error Need thread implementation for this platform
#include "generic/SDL_systhread_c.h"
#endif

#include "../SDL_error_c.h"

/* This is the system-independent thread info structure */
struct SDL_Thread {
	Uint32 threadid;
	SYS_ThreadHandle handle;
	int status;
	SDL_error errbuf;
	void *data;
};

/* This is the function called to run a thread */
extern void SDL_RunThread(void *data);

#endif /* SDL_thread_c_h_ */
