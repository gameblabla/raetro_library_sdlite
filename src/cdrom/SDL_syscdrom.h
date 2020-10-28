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

/* This is the system specific header for the SDL CD-ROM API */

/* Structure of CD audio control functions */
extern struct CDcaps {
	/* Get the name of the specified drive */
	const char *(*Name)(int drive);

	/* Open the specified drive, returning a drive id, or -1 on error */
	int (*Open)(int drive);

	/* Get table-of-contents (number of tracks + track info) for disk.
	   The TOC information should be stored in the cdrom structure.
	   This function should return 0 on success, or -1 on error.
	 */
	int (*GetTOC)(SDL_CD *cdrom);

	/* Return the current status and play position, in frames, of the
	   drive.  'position' may be NULL, and if so, should be ignored.
	 */
	CDstatus (*Status)(SDL_CD *cdrom, int *position);

	/* Play from frame 'start' to 'start+len' */
	int (*Play)(SDL_CD *cdrom, int start, int len);

	/* Pause play */
	int (*Pause)(SDL_CD *cdrom);

	/* Resume play */
	int (*Resume)(SDL_CD *cdrom);

	/* Stop play */
	int (*Stop)(SDL_CD *cdrom);

	/* Eject the current disk */
	int (*Eject)(SDL_CD *cdrom);

	/* Close the specified drive */
	void (*Close)(SDL_CD *cdrom);
} SDL_CDcaps;

/* The number of available CD-ROM drives on the system */
extern int SDL_numcds;

/* Function to scan the system for CD-ROM drives and fill SDL_CDcaps.
 * This function should set SDL_numcds to the number of available CD
 * drives.  Drive 0 should be the system default CD-ROM.
 * It should return 0, or -1 on an unrecoverable fatal error.
*/
extern int SDL_SYS_CDInit(void);

/* Function to perform any system-specific CD-ROM related cleanup */
extern void SDL_SYS_CDQuit(void);

