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

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_assert.h"
#include "SDL_assert_c.h"
#include "video/SDL_sysvideo.h"
#include <stdio.h>
#include <stdlib.h>

static SDL_assert_state SDLCALL
SDL_PromptAssertion(const SDL_assert_data *data, void *userdata);

/*
 * We keep all triggered assertions in a singly-linked list so we can
 *  generate a report later.
 */
static SDL_assert_data *triggered_assertions = NULL;

#ifndef SDL_THREADS_DISABLED
static SDL_mutex *assertion_mutex = NULL;
#endif

static SDL_AssertionHandler assertion_handler = SDL_PromptAssertion;
static void *assertion_userdata = NULL;

#ifdef __GNUC__

static void debug_print(const char *fmt, ...) __attribute__((format (printf, 1, 2)));

#endif

static void debug_print(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_ASSERT, SDL_LOG_PRIORITY_WARN, fmt, ap);
	va_end(ap);
}

static void SDL_AddAssertionToReport(SDL_assert_data *data) {
	/* (data) is always a static struct defined with the assert macros, so
	   we don't have to worry about copying or allocating them. */
	data->trigger_count++;
	if(data->trigger_count == 1) {  /* not yet added? */
		data->next = triggered_assertions;
		triggered_assertions = data;
	}
}

static void SDL_GenerateAssertionReport(void) {
	const SDL_assert_data *item = triggered_assertions;

	/* only do this if the app hasn't assigned an assertion handler. */
	if((item != NULL) && (assertion_handler != SDL_PromptAssertion)) {
		debug_print("\n\nSDL assertion report.\n");
		debug_print("All SDL assertions between last init/quit:\n\n");

		while (item != NULL) {
			debug_print("'%s'\n"
						"    * %s (%s:%d)\n"
						"    * triggered %u time%s.\n"
						"    * always ignore: %s.\n", item->condition, item->function, item->filename, item->linenum, item->trigger_count, (item->trigger_count == 1) ? "" : "s", item->always_ignore ? "yes" : "no");
			item = item->next;
		}
		debug_print("\n");

		SDL_ResetAssertionReport();
	}
}

extern SDL_NORETURN void SDL_ExitProcess(int exitcode);

static SDL_NORETURN void SDL_AbortAssertion(void) {
	SDL_Quit();
	SDL_ExitProcess(42);
}

static SDL_assert_state SDLCALL
SDL_PromptAssertion(const SDL_assert_data *data, void *userdata) {
#define ENDLINE "\n"

	const char *envr;
	SDL_assert_state state = SDL_ASSERTION_ABORT;
	char *message;

	(void) userdata;  /* unused in default handler. */

	/* !!! FIXME: why is this using SDL_stack_alloc and not just "char message[SDL_MAX_LOG_MESSAGE];" ? */
	message = SDL_stack_alloc(char, SDL_MAX_LOG_MESSAGE);
	if(!message) {
		/* Uh oh, we're in real trouble now... */
		return SDL_ASSERTION_ABORT;
	}
	SDL_snprintf(message, SDL_MAX_LOG_MESSAGE, "Assertion failure at %s (%s:%d), triggered %u %s:" ENDLINE
											   "  '%s'", data->function, data->filename, data->linenum, data->trigger_count, (data->trigger_count == 1) ? "time" : "times", data->condition);

	debug_print("\n\n%s\n\n", message);

	/* let env. variable override, so unit tests won't block in a GUI. */
	envr = SDL_getenv("SDL_ASSERT");
	if(envr != NULL) {
		SDL_stack_free(message);

		if(SDL_strcmp(envr, "abort") == 0) {
			return SDL_ASSERTION_ABORT;
		} else if(SDL_strcmp(envr, "break") == 0) {
			return SDL_ASSERTION_BREAK;
		} else if(SDL_strcmp(envr, "retry") == 0) {
			return SDL_ASSERTION_RETRY;
		} else if(SDL_strcmp(envr, "ignore") == 0) {
			return SDL_ASSERTION_IGNORE;
		} else if(SDL_strcmp(envr, "always_ignore") == 0) {
			return SDL_ASSERTION_ALWAYS_IGNORE;
		} else {
			return SDL_ASSERTION_ABORT;  /* oh well. */
		}
	}
	/* Re-enter fullscreen mode */
	SDL_stack_free(message);

	return state;
}

SDL_assert_state SDL_ReportAssertion(SDL_assert_data *data, const char *func, const char *file, int line) {
	SDL_assert_state state = SDL_ASSERTION_IGNORE;
	static int assertion_running = 0;

#ifndef SDL_THREADS_DISABLED
	if(assertion_mutex == NULL) { /* never called SDL_Init()? */
		assertion_mutex = SDL_CreateMutex();
		if(assertion_mutex == NULL) {
			return SDL_ASSERTION_IGNORE;   /* oh well, I guess. */
		}
	}
	if(SDL_LockMutex(assertion_mutex) < 0) {
		return SDL_ASSERTION_IGNORE;   /* oh well, I guess. */
	}
#endif
	/* doing this because Visual C is upset over assigning in the macro. */
	if(data->trigger_count == 0) {
		data->function = func;
		data->filename = file;
		data->linenum = line;
	}

	SDL_AddAssertionToReport(data);

	assertion_running++;
	if(assertion_running > 1) {   /* assert during assert! Abort. */
		if(assertion_running == 2) {
			SDL_AbortAssertion();
		} else if(assertion_running == 3) {  /* Abort asserted! */
			SDL_ExitProcess(42);
		} else {
			while (1) { /* do nothing but spin; what else can you do?! */ }
		}
	}

	if(!data->always_ignore) {
		state = assertion_handler(data, assertion_userdata);
	}

	switch (state) {
		case SDL_ASSERTION_ALWAYS_IGNORE:
			state = SDL_ASSERTION_IGNORE;
			data->always_ignore = 1;
			break;

		case SDL_ASSERTION_IGNORE:
		case SDL_ASSERTION_RETRY:
		case SDL_ASSERTION_BREAK:
			break;  /* macro handles these. */

		case SDL_ASSERTION_ABORT:
			SDL_AbortAssertion();
			/*break;  ...shouldn't return, but oh well. */
	}

	assertion_running--;

#ifndef SDL_THREADS_DISABLED
	SDL_UnlockMutex(assertion_mutex);
#endif

	return state;
}

void SDL_AssertionsQuit(void) {
	SDL_GenerateAssertionReport();
#ifndef SDL_THREADS_DISABLED
	if(assertion_mutex != NULL) {
		SDL_DestroyMutex(assertion_mutex);
		assertion_mutex = NULL;
	}
#endif
}

void SDL_SetAssertionHandler(SDL_AssertionHandler handler, void *userdata) {
	if(handler != NULL) {
		assertion_handler = handler;
		assertion_userdata = userdata;
	} else {
		assertion_handler = SDL_PromptAssertion;
		assertion_userdata = NULL;
	}
}

const SDL_assert_data *SDL_GetAssertionReport(void) {
	return triggered_assertions;
}

void SDL_ResetAssertionReport(void) {
	SDL_assert_data *next = NULL;
	SDL_assert_data *item;
	for (item = triggered_assertions; item != NULL; item = next) {
		next = (SDL_assert_data *) item->next;
		item->always_ignore = SDL_FALSE;
		item->trigger_count = 0;
		item->next = NULL;
	}

	triggered_assertions = NULL;
}

SDL_AssertionHandler SDL_GetDefaultAssertionHandler(void) {
	return SDL_PromptAssertion;
}

SDL_AssertionHandler SDL_GetAssertionHandler(void **userdata) {
	if(userdata != NULL) {
		*userdata = assertion_userdata;
	}
	return assertion_handler;
}
