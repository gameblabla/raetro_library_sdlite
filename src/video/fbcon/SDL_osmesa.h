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
 * @file SDL_osmesa.h
 *
 * Jens Thiele
 * karme@berlios.de
 *
 * This is based upon SDL_x11gl_c.h
 *
 * @details SDL_osmesa.h and SDL_osmesa.c integrate osmesa into SDL
 * osmesa is the offscreen rendering backend of mesa (http://www.mesa3d.org)
 *
 * This could be used not only by the fbcon SDL target but
 * by all targets without an hardware accelerated OpenGL driver
 * that are supported by osmesa and SIGTERM (system termination request),
 * if handlers do not already exist, that generate SDL_QUITEVENT events as well.
 * There is no way to determine the cause of an SDL_QUITEVENT, but setting a signal
 * handler in your application will override the default generation of
 * quit events for that signal.
 *
 * There are no functions directly affecting the quit event
 *
 ******************************************************************************/
#ifdef HAVE_OSMESA
#include <GL/osmesa.h>
#include <dlfcn.h>
#endif

#include "SDL_video.h"

struct SDL_PrivateGLData {
	int gl_active; /* to stop switching drivers while we have a valid context */

#ifdef HAVE_OSMESA
	OSMesaContext osmesa_context; /* Current OSMesa context */
	SDL_Surface *osmesa_buffer;    /* This is the surface we let OSMESA render to */

	void *(*OSMesaGetProcAddress)(const GLubyte *procName);

	OSMesaContext (*OSMesaCreateContext)(GLenum format, OSMesaContext sharelist);

	void (*OSMesaDestroyContext)(OSMesaContext ctx);

	GLboolean (*OSMesaMakeCurrent)(OSMesaContext ctx, void *buffer, GLenum type, GLsizei width, GLsizei height);

	void (*OSMesaPixelStore)(GLint pname, GLint value);

	void (*glFinish)(void);

#endif /* HAVE_OSMESA */
};

/* OpenGL functions */
#ifdef HAVE_OSMESA

/**
 * Those are used by function pointers
 * (implementations of the corresponding SDL functions)
 */
extern int SDL_OSMesa_LoadLibrary(_THIS, const char* path);
extern void *SDL_OSMesa_GetProcAddress(_THIS, const char* proc);
extern int SDL_OSMesa_GetAttribute(_THIS, SDL_GLattr attrib, int* value);
extern int SDL_OSMesa_MakeCurrent(_THIS);
extern void SDL_OSMesa_SwapBuffers(_THIS);

#endif

extern int SDL_OSMesa_CreateContext(_THIS, SDL_Surface *screen);

extern void SDL_OSMesa_Shutdown(_THIS);

extern void SDL_OSMesa_UnloadLibrary(_THIS);
