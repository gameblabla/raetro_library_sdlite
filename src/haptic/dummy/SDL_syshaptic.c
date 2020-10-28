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
 * Copyright (c) 2008 Edgar Simo
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

#include "SDL_haptic.h"
#include "../SDL_syshaptic.h"

static int SDL_SYS_LogicError(void) {
	SDL_SetError("Logic error: No haptic devices available.");
	return 0;
}

int SDL_SYS_HapticInit(void) {
	return 0;
}

const char *SDL_SYS_HapticName(int index) {
	SDL_SYS_LogicError();
	return NULL;
}

int SDL_SYS_HapticOpen(SDL_Haptic *haptic) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticMouse(void) {
	return -1;
}

#if 0 /* No Joystick feedback support in hsdl. */
int SDL_SYS_JoystickIsHaptic(SDL_Joystick *joystick) {
	return 0;
}

int SDL_SYS_HapticOpenFromJoystick(SDL_Haptic *haptic, SDL_Joystick *joystick) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_JoystickSameHaptic(SDL_Haptic *haptic, SDL_Joystick *joystick) {
	return 0;
}

#endif

void SDL_SYS_HapticClose(SDL_Haptic *haptic) {
	return;
}

void SDL_SYS_HapticQuit(void) {
	return;
}

int SDL_SYS_HapticNewEffect(SDL_Haptic *haptic, struct haptic_effect *effect, SDL_HapticEffect *base) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticUpdateEffect(SDL_Haptic *haptic, struct haptic_effect *effect, SDL_HapticEffect *data) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticRunEffect(SDL_Haptic *haptic, struct haptic_effect *effect, Uint32 iterations) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticStopEffect(SDL_Haptic *haptic, struct haptic_effect *effect) {
	SDL_SYS_LogicError();
	return -1;
}

void SDL_SYS_HapticDestroyEffect(SDL_Haptic *haptic, struct haptic_effect *effect) {
	SDL_SYS_LogicError();
	return;
}

int SDL_SYS_HapticGetEffectStatus(SDL_Haptic *haptic, struct haptic_effect *effect) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticSetGain(SDL_Haptic *haptic, int gain) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticSetAutocenter(SDL_Haptic *haptic, int autocenter) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticPause(SDL_Haptic *haptic) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticUnpause(SDL_Haptic *haptic) {
	SDL_SYS_LogicError();
	return -1;
}

int SDL_SYS_HapticStopAll(SDL_Haptic *haptic) {
	SDL_SYS_LogicError();
	return -1;
}
