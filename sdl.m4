dnl ###################################################################################
dnl  Project       : SDLite 1.2.x
dnl  Module        : Core
dnl  Purpose       : Provide Low-level access to framebuffer, audio output and HID.
dnl  License       : GNU General Public License v3.0
dnl ###################################################################################
dnl 
dnl  This file is part of the SDLite 1.2.x - SDK for Embedded Systems.
dnl 
dnl  Copyright (c) 2019-2020 Marcus Andrade
dnl  Copyright (c) 1997-2012 Sam Lantinga <slouken@libsdl.org>
dnl 
dnl  This library is free software: you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation, version 3.
dnl 
dnl  This library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
dnl  General Public License for more details.
dnl 
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.
dnl 
dnl ###################################################################################
dnl Shamelessly stolen from everyone below
dnl Sam Lantinga, Frank Belew, Manish Singh and Owen Taylor
dnl
dnl AM_PATH_SDL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for SDL, and define SDL_CFLAGS and SDL_LIBS as appropriate.
dnl enables arguments --with-sdl-prefix=
dnl                   --with-sdl-exec-prefix=
dnl
dnl Get the cflags and libraries from the sdl-config script
AC_DEFUN([AM_PATH_SDL], [ 
    AC_ARG_WITH(sdl-prefix,[  --with-sdl-prefix=PFX    Prefix where SDLite is installed (optional)], sdl_prefix="$withval", sdl_prefix="")
    AC_ARG_WITH(sdl-exec-prefix,[  --with-sdl-exec-prefix=PFX    Exec prefix where SDLite is installed (optional)], sdl_exec_prefix="$withval", sdl_exec_prefix="")

    if test x$sdl_exec_prefix != x ; then
        sdl_config_args="$sdl_config_args --exec-prefix=$sdl_exec_prefix"
        if test x${SDL_CONFIG+set} != xset ; then
        SDL_CONFIG=$sdl_exec_prefix/bin/sdl-config
        fi
    fi

    if test x$sdl_prefix != x ; then
        sdl_config_args="$sdl_config_args --prefix=$sdl_prefix"
        if test x${SDL_CONFIG+set} != xset ; then
        SDL_CONFIG=$sdl_prefix/bin/sdl-config
        fi
    fi

    as_save_PATH="$PATH"
    if test "x$prefix" != xNONE && test "$cross_compiling" != yes; then
        PATH="$prefix/bin:$prefix/usr/bin:$PATH"
    fi

    AC_PATH_PROG(SDL_CONFIG, sdl-config, no, [$PATH])
    PATH="$as_save_PATH"
    min_sdl_version=ifelse([$1], ,0.11.0,$1)
    AC_MSG_CHECKING(for SDLite 1.2.x - version >= $min_sdl_version)
    
    if test "$SDL_CONFIG" = "no" ; then
        echo "*** The sdl-config script installed by SDL could not be found"
        echo "*** If SDL was installed in PREFIX, make sure PREFIX/bin is in"
        echo "*** your path, or set the SDL_CONFIG environment variable to the"
        echo "*** full path to sdl-config."
        SDL_CFLAGS=""
        SDL_LIBS=""
        ifelse([$3], , :, [$3])
        AC_MSG_RESULT(no)
    else
        SDL_CFLAGS=`$SDL_CONFIG $sdl_config_args --cflags`
        SDL_LIBS=`$SDL_CONFIG $sdl_config_args --libs`
        sdl_major_version=`$SDL_CONFIG $sdl_config_args --version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
        sdl_minor_version=`$SDL_CONFIG $sdl_config_args --version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
        sdl_micro_version=`$SDL_CONFIG $sdl_config_args --version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
        AC_MSG_RESULT(yes)
        ifelse([$2], , :, [$2])
    fi

    AC_SUBST(SDL_CFLAGS)
    AC_SUBST(SDL_LIBS)

])
