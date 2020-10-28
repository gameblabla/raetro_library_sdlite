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

/*!*****************************************************************************
 * @file SDL_thread.h
 * @brief Thread management.
 * @note These are independent of the other SDL routines.
 ******************************************************************************/

/*!*****************************************************************************
 * @defgroup threads Threads
 * @brief Multi-threaded Programming
 * @details Functions for creating threads, mutexes, semaphores and condition variables.
 * @{
 ******************************************************************************/
#ifndef SDL_thread_h_
#define SDL_thread_h_

#include "SDL_stdinc.h"
#include "SDL_error.h"

/* Thread synchronization primitives */
#include "SDL_mutex.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/** The SDL thread structure, defined in SDL_thread.c */
struct SDL_Thread;
typedef struct SDL_Thread SDL_Thread;

/** Create a thread */
#if ((defined(__WIN32__) && !defined(HAVE_LIBC)) || defined(__OS2__)) && !defined(__SYMBIAN32__)
/**
 *  We compile SDL into a DLL on OS/2. This means, that it's the DLL which
 *  creates a new thread for the calling process with the SDL_CreateThread()
 *  API. There is a problem with this, that only the RTL of the SDL.DLL will
 *  be initialized for those threads, and not the RTL of the calling application!
 *  To solve this, we make a little hack here.
 *  We'll always use the caller's _beginthread() and _endthread() APIs to
 *  start a new thread. This way, if it's the SDL.DLL which uses this API,
 *  then the RTL of SDL.DLL will be used to create the new thread, and if it's
 *  the application, then the RTL of the application will be used.
 *  So, in short:
 *  Always use the _beginthread() and _endthread() of the calling runtime library!
 */
#define SDL_PASSED_BEGINTHREAD_ENDTHREAD
#ifndef _WIN32_WCE
#include <process.h> /* This has _beginthread() and _endthread() defined! */
#endif
#ifdef __EMX__
#include <stdlib.h>
#endif

#ifdef __OS2__
typedef int (*pfnSDL_CurrentBeginThread)(void (*func)(void *), void *, unsigned, void *arg); 
typedef void (*pfnSDL_CurrentEndThread)(void);
#else
typedef uintptr_t (__cdecl *pfnSDL_CurrentBeginThread) (void *, unsigned,
		unsigned (__stdcall *func)(void *), void *arg,
		unsigned, unsigned *threadID);
typedef void (__cdecl *pfnSDL_CurrentEndThread)(unsigned code);
#endif

extern DECLSPEC SDL_Thread * SDLCALL SDL_CreateThread(int (SDLCALL *fn)(void *), void *data, pfnSDL_CurrentBeginThread pfnBeginThread, pfnSDL_CurrentEndThread pfnEndThread);

#ifdef __OS2__
#define SDL_CreateThread(fn, data) SDL_CreateThread(fn, data, _beginthread, _endthread)
#elif defined(_WIN32_WCE)
#define SDL_CreateThread(fn, data) SDL_CreateThread(fn, data, NULL, NULL)
#else
#define SDL_CreateThread(fn, data) SDL_CreateThread(fn, data, _beginthreadex, _endthreadex)
#endif
#else

extern DECLSPEC SDL_Thread *SDLCALL SDL_CreateThread(int (SDLCALL *fn)(void *), void *data);

#endif

/** Get the 32-bit thread identifier for the current thread */
extern DECLSPEC Uint32 SDLCALL SDL_ThreadID(void);

/** Get the 32-bit thread identifier for the specified thread,
 *  equivalent to SDL_ThreadID() if the specified thread is NULL.
 */
extern DECLSPEC Uint32 SDLCALL SDL_GetThreadID(SDL_Thread *thread);

/** Wait for a thread to finish.
 *  The return code for the thread function is placed in the area
 *  pointed to by 'status', if 'status' is not NULL.
 */
extern DECLSPEC void SDLCALL SDL_WaitThread(SDL_Thread *thread, int *status);

/** Forcefully kill a thread without worrying about its state */
extern DECLSPEC void SDLCALL SDL_KillThread(SDL_Thread *thread);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include "close_code.h"

#endif /* SDL_thread_h_ */
