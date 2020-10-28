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

/* WAVE files are little-endian */

/*******************************************/
/* Define values for Microsoft WAVE format */
/*******************************************/
#define RIFF        0x46464952        /* "RIFF" */
#define WAVE        0x45564157        /* "WAVE" */
#define FACT        0x74636166        /* "fact" */
#define LIST        0x5453494c        /* "LIST" */
#define FMT        0x20746D66        /* "fmt " */
#define DATA        0x61746164        /* "data" */
#define PCM_CODE    0x0001
#define MS_ADPCM_CODE    0x0002
#define IMA_ADPCM_CODE    0x0011
#define MP3_CODE    0x0055
#define WAVE_MONO    1
#define WAVE_STEREO    2

/* Normally, these three chunks come consecutively in a WAVE file */
typedef struct WaveFMT {
	/* Not saved in the chunk we read:
		Uint32	FMTchunk;
		Uint32	fmtlen;
	*/
	Uint16 encoding;
	Uint16 channels;        /* 1 = mono, 2 = stereo */
	Uint32 frequency;        /* One of 11025, 22050, or 44100 Hz */
	Uint32 byterate;        /* Average bytes per second */
	Uint16 blockalign;        /* Bytes per sample block */
	Uint16 bitspersample;        /* One of 8, 12, 16, or 4 for ADPCM */
} WaveFMT;

/* The general chunk found in the WAVE file */
typedef struct Chunk {
	Uint32 magic;
	Uint32 length;
	Uint8 *data;
} Chunk;

