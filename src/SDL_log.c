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
#include "SDL.h"

/* Simple log messages in SDL */

#include "SDL_error.h"
#include "SDL_log.h"

#if HAVE_STDIO_H

#include <stdio.h>

#endif

#define DEFAULT_PRIORITY                SDL_LOG_PRIORITY_CRITICAL
#define DEFAULT_ASSERT_PRIORITY         SDL_LOG_PRIORITY_WARN
#define DEFAULT_APPLICATION_PRIORITY    SDL_LOG_PRIORITY_INFO
#define DEFAULT_TEST_PRIORITY           SDL_LOG_PRIORITY_VERBOSE

typedef struct SDL_LogLevel {
	int category;
	SDL_LogPriority priority;
	struct SDL_LogLevel *next;
} SDL_LogLevel;

/* The default log output function */
static void SDLCALL SDL_LogOutput(void *userdata, int category, SDL_LogPriority priority, const char *message);

static SDL_LogLevel *SDL_loglevels;
static SDL_LogPriority SDL_default_priority = DEFAULT_PRIORITY;
static SDL_LogPriority SDL_assert_priority = DEFAULT_ASSERT_PRIORITY;
static SDL_LogPriority SDL_application_priority = DEFAULT_APPLICATION_PRIORITY;
static SDL_LogPriority SDL_test_priority = DEFAULT_TEST_PRIORITY;
static SDL_LogOutputFunction SDL_log_function = SDL_LogOutput;
static void *SDL_log_userdata = NULL;

static const char *SDL_priority_prefixes[SDL_NUM_LOG_PRIORITIES] = {
	NULL,
	"VERBOSE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"CRITICAL"
};

void SDL_LogSetAllPriority(SDL_LogPriority priority) {
	SDL_LogLevel *entry;

	for (entry = SDL_loglevels; entry; entry = entry->next) {
		entry->priority = priority;
	}
	SDL_default_priority = priority;
	SDL_assert_priority = priority;
	SDL_application_priority = priority;
}

void SDL_LogSetPriority(int category, SDL_LogPriority priority) {
	SDL_LogLevel *entry;

	for (entry = SDL_loglevels; entry; entry = entry->next) {
		if(entry->category == category) {
			entry->priority = priority;
			return;
		}
	}

	/* Create a new entry */
	entry = (SDL_LogLevel *) SDL_malloc(sizeof(*entry));
	if(entry) {
		entry->category = category;
		entry->priority = priority;
		entry->next = SDL_loglevels;
		SDL_loglevels = entry;
	}
}

SDL_LogPriority SDL_LogGetPriority(int category) {
	SDL_LogLevel *entry;

	for (entry = SDL_loglevels; entry; entry = entry->next) {
		if(entry->category == category) {
			return entry->priority;
		}
	}

	if(category == SDL_LOG_CATEGORY_TEST) {
		return SDL_test_priority;
	} else if(category == SDL_LOG_CATEGORY_APPLICATION) {
		return SDL_application_priority;
	} else if(category == SDL_LOG_CATEGORY_ASSERT) {
		return SDL_assert_priority;
	} else {
		return SDL_default_priority;
	}
}

void SDL_LogResetPriorities(void) {
	SDL_LogLevel *entry;

	while (SDL_loglevels) {
		entry = SDL_loglevels;
		SDL_loglevels = entry->next;
		SDL_free(entry);
	}

	SDL_default_priority = DEFAULT_PRIORITY;
	SDL_assert_priority = DEFAULT_ASSERT_PRIORITY;
	SDL_application_priority = DEFAULT_APPLICATION_PRIORITY;
	SDL_test_priority = DEFAULT_TEST_PRIORITY;
}

void SDL_Log(SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ap);
	va_end(ap);
}

void SDL_LogVerbose(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_VERBOSE, fmt, ap);
	va_end(ap);
}

void SDL_LogDebug(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_DEBUG, fmt, ap);
	va_end(ap);
}

void SDL_LogInfo(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_INFO, fmt, ap);
	va_end(ap);
}

void SDL_LogWarn(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_WARN, fmt, ap);
	va_end(ap);
}

void SDL_LogError(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_ERROR, fmt, ap);
	va_end(ap);
}

void SDL_LogCritical(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_CRITICAL, fmt, ap);
	va_end(ap);
}

void SDL_LogMessage(int category, SDL_LogPriority priority, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	SDL_LogMessageV(category, priority, fmt, ap);
	va_end(ap);
}

void SDL_LogMessageV(int category, SDL_LogPriority priority, const char *fmt, va_list ap) {
	char *message;
	size_t len;

	/* Nothing to do if we don't have an output function */
	if(!SDL_log_function) {
		return;
	}

	/* Make sure we don't exceed array bounds */
	if((int) priority < 0 || priority >= SDL_NUM_LOG_PRIORITIES) {
		return;
	}

	/* See if we want to do anything with this message */
	if(priority < SDL_LogGetPriority(category)) {
		return;
	}

	/* !!! FIXME: why not just "char message[SDL_MAX_LOG_MESSAGE];" ? */
	message = SDL_stack_alloc(char, SDL_MAX_LOG_MESSAGE);
	if(!message) {
		return;
	}

	SDL_vsnprintf(message, SDL_MAX_LOG_MESSAGE, fmt, ap);

	/* Chop off final endline. */
	len = SDL_strlen(message);
	if((len > 0) && (message[len - 1] == '\n')) {
		message[--len] = '\0';
		if((len > 0) && (message[len - 1] == '\r')) {  /* catch "\r\n", too. */
			message[--len] = '\0';
		}
	}

	SDL_log_function(SDL_log_userdata, category, priority, message);
	SDL_stack_free(message);
}

static void SDLCALL SDL_LogOutput(void *userdata, int category, SDL_LogPriority priority, const char *message) {
#if HAVE_STDIO_H
	fprintf(stderr, "%s: %s\n", SDL_priority_prefixes[priority], message);
#if __NACL__
	fflush(stderr);
#endif
#endif
}

void SDL_LogGetOutputFunction(SDL_LogOutputFunction *callback, void **userdata) {
	if(callback) {
		*callback = SDL_log_function;
	}
	if(userdata) {
		*userdata = SDL_log_userdata;
	}
}

void SDL_LogSetOutputFunction(SDL_LogOutputFunction callback, void *userdata) {
	SDL_log_function = callback;
	SDL_log_userdata = userdata;
}
