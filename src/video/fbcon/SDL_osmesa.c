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

#include <stdlib.h>    /* For getenv() prototype */
#include <string.h>
#include <assert.h>

#include "SDL_fbvideo.h"

#include "SDL_events.h"
#include "SDL_error.h"
#include "SDL_osmesa.h"

#define DEFAULT_OPENGL "libOSMesa.so"

/* member variable names */
#define gl_active      (this->gl_data->gl_active)
#define osmesa_context (this->gl_data->osmesa_context)
#define osmesa_buffer  (this->gl_data->osmesa_buffer)

int SDL_OSMesa_CreateContext(_THIS, SDL_Surface *screen) {
	int retval;
#ifdef HAVE_OSMESA
	assert(screen);

	if(!this->gl_config.driver_loaded) {
		/* no driver has been loaded, use default (ourselves) */
		if(SDL_OSMesa_LoadLibrary(this, NULL) < 0) {
			return -1;
		}
	}
	/**
	 * osmesa (mesa ver. 5) does support following pixel formats:
	 * OSMESA_COLOR_INDEX:  8Bit palette
	 * OSMESA_RGBA:         4*8 Bit
	 * OSMESA_BGRA
	 * OSMESA_ARGB
	 * OSMESA_RGB:          3*8 Bit
	 * OSMESA_BGR
	 * OSMESA_RGB_565       16Bit
	 * for now we use only OSMESA_RGBA and OSMESA_RGB_565
	*/
	switch (screen->format->BitsPerPixel) {
#ifdef OSMESA_RGB_565
		case 16:
			osmesa_buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
			break;
#endif
		default:
			osmesa_buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			0x000000ff, 0x0000ff00, 0x00ff0000,
#else
			0xff000000, 0x00ff0000, 0x0000ff00,
#endif
			0x00000000 /* TODO: should we use the alpha-channel ? */
			);
			break;
	};

	if(osmesa_buffer == NULL) {
		SDL_SetError("Could not create software surface");
		return -1;
	}

	/* safety check because we do not lock this surface */
	if(SDL_MUSTLOCK(osmesa_buffer)) {
		SDL_SetError("surface needs lock");
		return -1;
	}

	/* now create a osmesa context which fits our osmesa_buffer */
	switch (osmesa_buffer->format->BitsPerPixel) {
		#ifdef OSMESA_RGB_565
		case 16:
		  osmesa_context = this->gl_data->OSMesaCreateContext(OSMESA_RGB_565,NULL);
		  break;
		#endif
		default:
			osmesa_context = this->gl_data->OSMesaCreateContext(OSMESA_RGBA, NULL);
			break;
	};

	if(osmesa_context == NULL) {
		SDL_FreeSurface(osmesa_buffer);
		osmesa_buffer = NULL;
		SDL_SetError("Could not create OSMesa context");
		return -1;
	}

	gl_active = 1;
	#else
	SDL_SetError("driver not configured with OSMesa");
	#endif
	if(gl_active) {
		retval = 0;
	} else {
		retval = -1;
	}
	return (retval);
}

void SDL_OSMesa_Shutdown(_THIS) {
	#ifdef HAVE_OSMESA
	/* Clean up OpenGL */
	if(osmesa_context) {
		this->gl_data->OSMesaDestroyContext(osmesa_context);
		osmesa_context = NULL;
	}
	if(osmesa_buffer) {
		SDL_FreeSurface(osmesa_buffer);
		osmesa_buffer = NULL;
	}
	gl_active = 0;
	#endif /* HAVE_OSMESA */
}

#ifdef HAVE_OSMESA

/* Make the current context active */
int SDL_OSMesa_MakeCurrent(_THIS) {
	int retval;
	switch (osmesa_buffer->format->BitsPerPixel) {
#ifdef OSMESA_RGB_565
		case 16:
			retval = this->gl_data->OSMesaMakeCurrent(osmesa_context, osmesa_buffer->pixels, GL_UNSIGNED_SHORT_5_6_5, osmesa_buffer->w, osmesa_buffer->h);
			break;
#endif
		default:
			retval = this->gl_data->OSMesaMakeCurrent(osmesa_context, osmesa_buffer->pixels, GL_UNSIGNED_BYTE, osmesa_buffer->w, osmesa_buffer->h);
			break;
	};

	if(!retval) {
		SDL_SetError("Unable to make GL context current");
		retval = -1;
	} else {
		/**
		 * Tell mesa that the first screen row is stored first (normal memory order)
		 * OSMesaPixelStore operates on the current mesa context and crashes if there is no current context !!
		 */
		this->gl_data->OSMesaPixelStore(OSMESA_Y_UP, 0);
		retval = 0;
	}
	return (retval);
}

/* Get attribute data from OSMesa. */
int SDL_OSMesa_GetAttribute(_THIS, SDL_GLattr attrib, int *value) {
	return -1;
#if 0
	/* TODO */
	int retval;
	int glx_attrib = None;

	switch( attrib ) {
	case SDL_GL_RED_SIZE:
	  glx_attrib = GLX_RED_SIZE;
	  break;
	case SDL_GL_GREEN_SIZE:
	  glx_attrib = GLX_GREEN_SIZE;
	  break;
	case SDL_GL_BLUE_SIZE:
	  glx_attrib = GLX_BLUE_SIZE;
	  break;
	case SDL_GL_ALPHA_SIZE:
	  glx_attrib = GLX_ALPHA_SIZE;
	  break;
	case SDL_GL_DOUBLEBUFFER:
	  glx_attrib = GLX_DOUBLEBUFFER;
	  break;
	case SDL_GL_BUFFER_SIZE:
	  glx_attrib = GLX_BUFFER_SIZE;
	  break;
	case SDL_GL_DEPTH_SIZE:
	  glx_attrib = GLX_DEPTH_SIZE;
	  break;
	case SDL_GL_STENCIL_SIZE:
	  glx_attrib = GLX_STENCIL_SIZE;
	  break;
	case SDL_GL_ACCUM_RED_SIZE:
	  glx_attrib = GLX_ACCUM_RED_SIZE;
	  break;
	case SDL_GL_ACCUM_GREEN_SIZE:
	  glx_attrib = GLX_ACCUM_GREEN_SIZE;
	  break;
	case SDL_GL_ACCUM_BLUE_SIZE:
	  glx_attrib = GLX_ACCUM_BLUE_SIZE;
	  break;
	case SDL_GL_ACCUM_ALPHA_SIZE:
	  glx_attrib = GLX_ACCUM_ALPHA_SIZE;
	  break;
	case SDL_GL_STEREO:
	  glx_attrib = GLX_STEREO;
	  break;
	default:
	  return(-1);
	}

	retval = this->gl_data->OSMesaGetConfig(GFX_Display, glx_visualinfo, glx_attrib, value);

	return retval;
#endif
}

void SDL_OSMesa_SwapBuffers(_THIS) {
	SDL_Rect oclip;
	SDL_Surface *screen = SDL_GetVideoSurface();

	/* tell mesa to paint everything - see man glFinish */
	this->gl_data->glFinish();

	/* make backup of clipping area */
	SDL_GetClipRect(screen, &oclip);

	/* clip to full screen */
	SDL_SetClipRect(screen, NULL);
	SDL_BlitSurface(osmesa_buffer, NULL, screen, NULL);
	SDL_SetClipRect(screen, &oclip);
	SDL_Flip(screen);
}

#endif /* HAVE_OSMESA */

void SDL_OSMesa_UnloadLibrary(_THIS) {
#ifdef HAVE_OSMESA
	if(this->gl_config.driver_loaded) {
		dlclose(this->gl_config.dll_handle);

		this->gl_data->OSMesaGetProcAddress = NULL;
		this->gl_data->OSMesaCreateContext = NULL;
		this->gl_data->OSMesaDestroyContext = NULL;
		this->gl_data->OSMesaMakeCurrent = NULL;
		this->gl_data->OSMesaPixelStore = NULL;
		this->gl_data->glFinish = NULL;
		this->gl_config.dll_handle = NULL;
		this->gl_config.driver_loaded = 0;
	}
#endif
}

#ifdef HAVE_OSMESA

/**
 * Passing a NULL path means:
 * Try to load pointers from the application
 * if this fails use the SDL_VIDEO_GL_DRIVER environment variable as library name
 * if this fails use the default library name
 */
int SDL_OSMesa_LoadLibrary(_THIS, const char *path) {
	void *handle;
	int dlopen_flags;

	if(gl_active) {
		SDL_SetError("OpenGL context already created");
		return -1;
	}

#ifdef RTLD_GLOBAL
	dlopen_flags = RTLD_LAZY | RTLD_GLOBAL;
#else
	dlopen_flags = RTLD_LAZY;
#endif
	handle = dlopen(path, dlopen_flags);
	/* Catch the case where the application isn't linked with GL */
	if((dlsym(handle, "OSMesaGetProcAddress") == NULL) && (path == NULL)) {
		dlclose(handle);
		path = getenv("SDL_VIDEO_GL_DRIVER");
		if(path == NULL) {
			path = DEFAULT_OPENGL;
		}
		handle = dlopen(path, dlopen_flags);
	}
	if(handle == NULL) {
		SDL_SetError("Could not load OpenGL library");
		return -1;
	}

	/* Unload the old driver and reset the pointers */
	SDL_OSMesa_UnloadLibrary(this);

	/* Load new function pointers */
	this->gl_data->OSMesaGetProcAddress = (void *(*)(const GLubyte *)) dlsym(handle, "OSMesaGetProcAddress");
	this->gl_data->OSMesaCreateContext = (OSMesaContext (*)(GLenum, OSMesaContext)) dlsym(handle, "OSMesaCreateContext");
	this->gl_data->OSMesaDestroyContext = (void (*)(OSMesaContext)) dlsym(handle, "OSMesaDestroyContext");
	this->gl_data->OSMesaMakeCurrent = (GLboolean (*)(OSMesaContext, void *, GLenum, GLsizei, GLsizei)) dlsym(handle, "OSMesaMakeCurrent");
	this->gl_data->OSMesaPixelStore = (void (*)(GLint, GLint)) dlsym(handle, "OSMesaPixelStore");
	this->gl_data->glFinish = (void (*)(void)) dlsym(handle, "glFinish");

	if((this->gl_data->OSMesaGetProcAddress == NULL) || (this->gl_data->OSMesaCreateContext == NULL) || (this->gl_data->OSMesaDestroyContext == NULL) || (this->gl_data->OSMesaMakeCurrent == NULL) || (this->gl_data->OSMesaPixelStore == NULL) || (this->gl_data->glFinish == NULL)) {
		SDL_SetError("Could not retrieve OpenGL functions");
		return -1;
	}

	this->gl_config.dll_handle = handle;
	this->gl_config.driver_loaded = 1;
	if(path) {
		strncpy(this->gl_config.driver_path, path, sizeof(this->gl_config.driver_path) - 1);
	} else {
		strcpy(this->gl_config.driver_path, "");
	}
	return 0;
}

void *SDL_OSMesa_GetProcAddress(_THIS, const char *proc) {
	assert(this);
	assert(this->gl_data);
	if(this->gl_data->OSMesaGetProcAddress) {
		return this->gl_data->OSMesaGetProcAddress(proc);
	}
	return NULL;
}

#endif /* HAVE_OSMESA */
