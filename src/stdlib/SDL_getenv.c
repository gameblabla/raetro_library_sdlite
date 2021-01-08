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
#include "SDL_stdinc.h"

#ifndef HAVE_GETENV

static char **SDL_env = (char **) 0;

/* Put a variable of the form "name=value" into the environment */
int SDL_putenv(const char *variable) {
	const char *name, *value;
	int added;
	int len, i;
	char **new_env;
	char *new_variable;

	/* A little error checking */
	if(!variable) {
		return (-1);
	}
	name = variable;
	for (value = variable; *value && (*value != '='); ++value) {
		/* Keep looking for '=' */ ;
	}
	if(*value) {
		++value;
	} else {
		return (-1);
	}

	/* Allocate memory for the variable */
	new_variable = SDL_strdup(variable);
	if(!new_variable) {
		return (-1);
	}

	/* Actually put it into the environment */
	added = 0;
	i = 0;
	if(SDL_env) {
		/* Check to see if it's already there... */
		len = (value - name);
		for (; SDL_env[i]; ++i) {
			if(SDL_strncmp(SDL_env[i], name, len) == 0) {
				break;
			}
		}
		/* If we found it, just replace the entry */
		if(SDL_env[i]) {
			SDL_free(SDL_env[i]);
			SDL_env[i] = new_variable;
			added = 1;
		}
	}

	/* Didn't find it in the environment, expand and add */
	if(!added) {
		new_env = SDL_realloc(SDL_env, (i + 2) * sizeof(char *));
		if(new_env) {
			SDL_env = new_env;
			SDL_env[i++] = new_variable;
			SDL_env[i++] = (char *) 0;
			added = 1;
		} else {
			SDL_free(new_variable);
		}
	}
	return (added ? 0 : -1);
}

/* Retrieve a variable named "name" from the environment */
char *SDL_getenv(const char *name) {
	int len, i;
	char *value;

	value = (char *) 0;
	if(SDL_env) {
		len = SDL_strlen(name);
		for (i = 0; SDL_env[i] && !value; ++i) {
			if((SDL_strncmp(SDL_env[i], name, len) == 0) && (SDL_env[i][len] == '=')) {
				value = &SDL_env[i][len + 1];
			}
		}
	}
	return value;
}

#endif /* !HAVE_GETENV */

#ifdef TEST_MAIN
#include <stdio.h>

int main(int argc, char *argv[]) {
	char *value;

	printf("Checking for non-existent variable... ");
	fflush(stdout);
	if ( ! SDL_getenv("EXISTS") ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	printf("Setting FIRST=VALUE1 in the environment... ");
	fflush(stdout);
	if ( SDL_putenv("FIRST=VALUE1") == 0 ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	printf("Getting FIRST from the environment... ");
	fflush(stdout);
	value = SDL_getenv("FIRST");
	if ( value && (SDL_strcmp(value, "VALUE1") == 0) ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	printf("Setting SECOND=VALUE2 in the environment... ");
	fflush(stdout);
	if ( SDL_putenv("SECOND=VALUE2") == 0 ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	printf("Getting SECOND from the environment... ");
	fflush(stdout);
	value = SDL_getenv("SECOND");
	if ( value && (SDL_strcmp(value, "VALUE2") == 0) ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	printf("Setting FIRST=NOVALUE in the environment... ");
	fflush(stdout);
	if ( SDL_putenv("FIRST=NOVALUE") == 0 ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	printf("Getting FIRST from the environment... ");
	fflush(stdout);
	value = SDL_getenv("FIRST");
	if ( value && (SDL_strcmp(value, "NOVALUE") == 0) ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	printf("Checking for non-existent variable... ");
	fflush(stdout);
	if ( ! SDL_getenv("EXISTS") ) {
		printf("okay\n");
	} else {
		printf("failed\n");
	}
	return(0);
}
#endif /* TEST_MAIN */

