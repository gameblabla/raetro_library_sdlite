dnl ###################################################################################
dnl Library       : SDLite 1.2.x
dnl Purpose       : Low-level access to a framebuffer, audio output and HID.
dnl Module        : Core
dnl Project       : Redux for Embedded System
dnl Description   : Stripped-down and optimized libraries for RISC processors
dnl License       : GNU General Public License v3.0
dnl ###################################################################################
dnl
dnl TinyRetroLabs and SDLite 1.2.x:
dnl Copyright (c) 2019-2020 Marcus Andrade <boogermann@tinyretrolabs.org>
dnl
dnl Simple DirectMedia Layer and SDL:
dnl Copyright (c) 1997-2012 Sam Lantinga <slouken@libsdl.org>
dnl
dnl This library is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, version 3.
dnl
dnl This library is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.
dnl If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.
dnl ###################################################################################
dnl Process this file with autoconf to produce a configure script.
dnl ###################################################################################
AC_PREREQ([2.69]) # Twice as Nice!!!
AC_INIT([SDL],[1.2.16])
AC_PREFIX_DEFAULT(/usr)
AC_CONFIG_HEADER(include/SDL_config.h)
AC_GNU_SOURCE
AC_CONFIG_AUX_DIRS($srcdir/build-scripts)

dnl Set various version strings - taken gratefully from the GTk sources
#
# Making releases:
# Edit include/SDL/SDL_version.h and change the version, then:
#   SDL_MICRO_VERSION += 1;
#   SDL_INTERFACE_AGE += 1;
#   SDL_BINARY_AGE += 1;
# if any functions have been added, set SDL_INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set SDL_BINARY_AGE and SDL_INTERFACE_AGE to 0.
#
AC_PREFIX_DEFAULT(/usr)
SDL_MAJOR_VERSION=1
SDL_MINOR_VERSION=2
SDL_MICRO_VERSION=16
SDL_INTERFACE_AGE=5
SDL_BINARY_AGE=16
SDL_VERSION=$SDL_MAJOR_VERSION.$SDL_MINOR_VERSION.$SDL_MICRO_VERSION

AC_SUBST(SDL_MAJOR_VERSION)
AC_SUBST(SDL_MINOR_VERSION)
AC_SUBST(SDL_MICRO_VERSION)
AC_SUBST(SDL_INTERFACE_AGE)
AC_SUBST(SDL_BINARY_AGE)
AC_SUBST(SDL_VERSION)

# libtool versioning
# LT_INIT([win32-dll])

LT_RELEASE=$SDL_MAJOR_VERSION.$SDL_MINOR_VERSION
LT_CURRENT=`expr $SDL_MICRO_VERSION - $SDL_INTERFACE_AGE`
LT_REVISION=$SDL_INTERFACE_AGE
LT_AGE=`expr $SDL_BINARY_AGE - $SDL_INTERFACE_AGE`
m4_pattern_allow([^LT_])

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

dnl Detect the canonical build and host environments
AC_CONFIG_AUX_DIR([build-scripts])
dnl AC_CANONICAL_HOST
AC_C_BIGENDIAN
if test x$ac_cv_c_bigendian = xyes; then
    AC_DEFINE(SDL_BYTEORDER, 4321)
else
    AC_DEFINE(SDL_BYTEORDER, 1234)
fi

dnl Check for tools
AC_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl Set up the compiler and linker flags
INCLUDE="-I$srcdir/include"
if test x$srcdir != x.; then
    # Remove SDL_config.h from the source directory, since it's the
    # default one, and we want to include the one that we generate.
    if test -f $srcdir/include/SDL_config.h; then
        rm $srcdir/include/SDL_config.h
    fi
    INCLUDE="-Iinclude $INCLUDE"
fi
case "$host" in
    *-*-cygwin*)
        # We build SDL on cygwin without the UNIX emulation layer
        BASE_CFLAGS="-I/usr/include/mingw -mno-cygwin"
        BASE_LDFLAGS="-mno-cygwin"
        ;;
    *)
        BASE_CFLAGS="-D_GNU_SOURCE=1"
        BASE_LDFLAGS=""
        ;;
esac
BUILD_CFLAGS="$CFLAGS $CPPFLAGS"
EXTRA_CFLAGS="$INCLUDE $BASE_CFLAGS"
BUILD_LDFLAGS="$LDFLAGS"
EXTRA_LDFLAGS="$BASE_LDFLAGS"
## These are common directories to find software packages
#for path in /usr/freeware /usr/pkg /usr/X11R6 /usr/local; do
#    if test -d $path/include; then
#        EXTRA_CFLAGS="$EXTRA_CFLAGS -I$path/include"
#    fi
#    if test -d $path/lib; then
#        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -L$path/lib"
#    fi
#done
SDL_CFLAGS="$BASE_CFLAGS"
SDL_LIBS="-lSDL $BASE_LDFLAGS"
CPPFLAGS="$CPPFLAGS $EXTRA_CFLAGS"
CFLAGS="$CFLAGS $EXTRA_CFLAGS"
LDFLAGS="$LDFLAGS $EXTRA_LDFLAGS"

dnl set this to use on systems that use lib64 instead of lib
base_libdir=`echo \${libdir} | sed 's/.*\/\(.*\)/\1/; q'`

dnl Function to find a library in the compiler search path
find_lib()
{
    gcc_bin_path=[`$CC -print-search-dirs 2>/dev/null | fgrep programs: | sed 's/[^=]*=\(.*\)/\1/' | sed 's/:/ /g'`]
    gcc_lib_path=[`$CC -print-search-dirs 2>/dev/null | fgrep libraries: | sed 's/[^=]*=\(.*\)/\1/' | sed 's/:/ /g'`]
    env_lib_path=[`echo $LIBS $LDFLAGS $* | sed 's/-L[ ]*//g'`]
    if test "$cross_compiling" = yes; then
        host_lib_path=""
    else
        host_lib_path="/usr/$base_libdir /usr/local/$base_libdir"
    fi
    for path in $env_lib_path $gcc_bin_path $gcc_lib_path $host_lib_path; do
        lib=[`ls -- $path/$1 2>/dev/null | sed -e '/\.so\..*\./d' -e 's,.*/,,' | sort | tail -1`]
        if test x$lib != x; then
            echo $lib
            return
        fi
    done
}

dnl Check for compiler characteristics
AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE
AC_C_BIGENDIAN

dnl ###################################################################################
dnl Check for a valid linux/version.h and generic linux input event support.
dnl ###################################################################################
AC_CHECK_HEADERS([linux/version.h],[EXTRA_CFLAGS="$EXTRA_CFLAGS -DHAVE_LINUX_VERSION_H"], [])
AC_CHECK_HEADERS([linux/input.h],  [AC_DEFINE([SDL_INPUT_LINUXEV], [1], [Define if supporting input events])], [])
dnl ###################################################################################
dnl Enable/disable various subsystems of the SDL library
dnl ###################################################################################
AC_ARG_ENABLE([libc],              [AS_HELP_STRING([--disable-libc],             [Disable Standard C libraries])],                     [enable_libc=$enableval],          [enable_libc=yes])
AC_ARG_ENABLE([audio],             [AS_HELP_STRING([--disable-audio],            [Disable Audio Subsystem])],                          [enable_audio=$enableval],         [enable_audio=yes])
AC_ARG_ENABLE([video],             [AS_HELP_STRING([--disable-video],            [Disable Video Subsystem])],                          [enable_video=$enableval],         [enable_video=yes])
AC_ARG_ENABLE([events],            [AS_HELP_STRING([--disable-events],           [Disable Events Subsystem])],                         [enable_events=$enableval],        [enable_events=yes])
AC_ARG_ENABLE([joystick],          [AS_HELP_STRING([--disable-joystick],         [Disable Joystick Subsystem])],                       [enable_joystick=$enableval],      [enable_joystick=yes])
AC_ARG_ENABLE([cdrom],             [AS_HELP_STRING([--disable-cdrom],            [Disable Cdrom Subsystem])],                          [enable_cdrom=$enableval],         [enable_cdrom=yes])
AC_ARG_ENABLE([threads],           [AS_HELP_STRING([--disable-threads],          [Disable Threading Subsystem])],                      [enable_threads=$enableval],       [enable_threads=yes])
AC_ARG_ENABLE([timers],            [AS_HELP_STRING([--disable-timers],           [Disable Timer Subsystem])],                          [enable_timers=$enableval],        [enable_timers=yes])
AC_ARG_ENABLE([file],              [AS_HELP_STRING([--disable-file],             [Disable File Subsystem])],                           [enable_file=$enableval],          [enable_file=yes])
AC_ARG_ENABLE([filesystem],        [AS_HELP_STRING([--disable-filesystem],       [Disable Filesystem Paths Subsystem])],               [enable_filesystem=$enableval],    [enable_filesystem=yes])
AC_ARG_ENABLE([loadso],            [AS_HELP_STRING([--disable-loadso],           [Disable Shared Object Loading Subsystem])],          [enable_loadso=$enableval],        [enable_loadso=yes])
AC_ARG_ENABLE([power],             [AS_HELP_STRING([--disable-power],            [Disable Power Management Subsystem])],               [enable_power=$enableval],         [enable_power=yes])
AC_ARG_ENABLE([cpuinfo],           [AS_HELP_STRING([--disable-cpuinfo],          [Disable Cpuinfo Subsystem ])],                       [enable_cpuinfo=$enableval],       [enable_cpuinfo=yes])
dnl ###################################################################################
AC_ARG_ENABLE([video-fbcon],       [AS_HELP_STRING([--disable-video-fbcon],      [Disable Framebuffer Console])],                      [enable_video_fbcon=$enableval],   [enable_video_fbcon=yes])
AC_ARG_ENABLE([video-dummy],       [AS_HELP_STRING([--disable-video-dummy],      [Disable Dummy Video Driver])],                       [enable_video_dummy=$enableval],   [enable_video_dummy=yes])
AC_ARG_ENABLE([video-kmsdrm],      [AS_HELP_STRING([--disable-video-kmsdrm],     [Disable KMSDRM Video Driver])],                      [enable_video_kmsdrm=$enableval],  [enable_video_kmsdrm=yes])
dnl ###################################################################################
AC_ARG_ENABLE([arm-simd],          [AS_HELP_STRING([--disable-arm-simd],         [Disable ARMv6 SIMD support])],                       [enable_arm_simd=$enableval],      [enable_arm_simd=yes])
AC_ARG_ENABLE([arm-neon],          [AS_HELP_STRING([--disable-arm-neon],         [Disable ARM NEON support])],                         [enable_arm_neon=$enableval],      [enable_arm_neon=yes])
dnl ###################################################################################
AC_ARG_ENABLE([oss],               [AS_HELP_STRING([--disable-oss],              [Disable OSS support])],                              [enable_oss=$enableval],           [enable_oss=yes])
AC_ARG_ENABLE([alsa],              [AS_HELP_STRING([--disable-alsa],             [Disable ALSA support])],                             [enable_alsa=$enableval],          [enable_alsa=yes])
AC_ARG_ENABLE([pulseaudio],        [AS_HELP_STRING([--disable-pulseaudio],       [Disable PulseAudio support])],                       [enable_pulseaudio=$enableval],    [enable_pulseaudio=yes])
AC_ARG_ENABLE([dummyaudio],        [AS_HELP_STRING([--disable-dummyaudio],       [Disable Dummy Audio Driver])],                       [enable_dummyaudio=$enableval],    [enable_dummyaudio=yes])
dnl ###################################################################################
AC_ARG_ENABLE([pthreads],          [AS_HELP_STRING([--disable-pthreads],         [Disable POSIX threads for multi-threading])],        [enable_pthreads=$enableval],      [enable_pthreads=yes])
AC_ARG_ENABLE([pthread-sem],       [AS_HELP_STRING([--disable-pthread-sem],      [Disable pthread semaphores])],                       [enable_pthread_sem=$enableval],   [enable_pthread_sem=yes])
AC_ARG_ENABLE([pth],               [AS_HELP_STRING([--enable-pth],               [Enable GNU pth library for multi-threading])],       [enable_pth=$enableval],           [enable_pth=no])
dnl ###################################################################################
AC_ARG_ENABLE([rpath],             [AS_HELP_STRING([--disable-rpath],            [Disable rpath when Linking SDL])],                   [enable_rpath=$enableval],         [enable_rpath=yes])
AC_ARG_ENABLE([sdl-dlopen],        [AS_HELP_STRING([--disable-sdl-dlopen],       [Disable dlopen for Shared Object loading])],         [enable_sdl_dlopen=$enableval],    [enable_sdl_dlopen=yes])
AC_ARG_ENABLE([alsa-shared],       [AS_HELP_STRING([--enable-alsa-shared],       [Disable Dynamically load ALSA support])],            [enable_oss=$enableval],           [enable_alsa_shared=yes])
AC_ARG_ENABLE([pulseaudio-shared], [AS_HELP_STRING([--enable-pulseaudio-shared], [Disable Dynamically load PulseAudio support])],      [enable_oss=$enableval],           [enable_pulseaudio_shared=yes])
dnl ###################################################################################
AC_ARG_ENABLE([clock_gettime],     [AS_HELP_STRING([--disable-clock_gettime],    [Enable gettimeofday() instead of clock_gettime()])], [enable_clock_gettime=$enableval], [enable_clock_gettime=yes])
AC_ARG_ENABLE(input-tslib,         [AS_HELP_STRING([--enable-input-tslib],       [Enable Touchscreen library for input])],             [enable_input_tslib=$enableval],   [enable_input_tslib=no])
AC_ARG_ENABLE([haptic],            [AS_HELP_STRING([--enable-haptic],            [Enable Haptic (force feedback) subsystem])],         [enable_haptic=$enableval],        [enable_haptic=no])
AC_ARG_ENABLE([screensaver],       [AS_HELP_STRING([--enable-screensaver],       [Enable Screensaver while an SDL app is running])],   [enable_screensaver=$enableval],   [enable_screensaver=no])
dnl ###################################################################################
AC_ARG_ENABLE([debug],             [AS_HELP_STRING([--enable-debug],             [Build Debug Version])],                              [use_debug=$enableval],            [use_debug=no])
AC_ARG_ENABLE([kmsdrm-debug],      [AS_HELP_STRING([--enable-kmsdrm-debug],      [Debug KMSDRM])],                                     [kmsdrm_debug=$enableval],         [kmsdrm_debug=no])
AC_ARG_WITH([platform],            [AS_HELP_STRING([--with-platform],            [Use Pre-configured Settings for SoC Boards])],       [use_platform=$withval],           [use_platform=none])
dnl ###################################################################################

if test "x$enable_libc" = "xyes"; then
    dnl Check for C library headers,  functions and libraries
    AC_DEFINE([HAVE_LIBC],[1],[Define if using C standard libraries])
    AC_HEADER_STDC
    AC_CHECK_HEADERS(sys/types.h sys/file.h sys/ioctl.h sys/mount.h \
        stddef.h stdint.h stdlib.h string.h strings.h stdio.h stdlib.h stddef.h stdarg.h \
        malloc.h memory.h string.h strings.h wchar.h inttypes.h stdint.h limits.h ctype.h \
        math.h float.h iconv.h signal.h libio.h unistd.h fcntl.h)

    AC_CHECK_FUNCS(malloc calloc realloc free getenv setenv putenv unsetenv getpagesize \
        qsort abs bcopy memset memcpy memmove wcslen wcslcpy wcslcat wcsdup wcsstr wcscmp \
        wcsncmp strlen strlcpy strlcat strdup _strrev _strupr _strlwr strchr strrchr strstr \
        strtok_r itoa _ltoa _uitoa _ultoa strtol strtoul _i64toa _ui64toa strtoll strtoull \
        atoi atof strcmp strncmp _stricmp strcasecmp _strnicmp strncasecmp vsscanf sscanf \
        snprintf vsnprintf iconv fopen64 fseeko fseeko64 sigaction setjmp nanosleep sysconf \
        sysctlbyname getauxval poll _Exit)

    AC_CHECK_FUNC([mprotect], [AC_DEFINE([HAVE_MPROTECT],[1],[Define if supporting mprotect])],[])

    AC_CHECK_LIB([iconv], [libiconv_open], [EXTRA_LDFLAGS="$EXTRA_LDFLAGS -liconv"])
    AC_CHECK_LIB([m], [pow], [EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lm"])
    AC_CHECK_MEMBER([struct sigaction.sa_sigaction],[AC_DEFINE([HAVE_SA_SIGACTION],[1],[Define if supporting sigaction])], ,[#include <signal.h>])
    AC_FUNC_ALLOCA
    AC_FUNC_MEMCMP
    AC_FUNC_STRTOD
    AC_TYPE_SIZE_T
    if test "x$ac_cv_header_inttypes_h" = "xyes" -o "x$ac_cv_header_stdint_h" = "xyes"; then
        AC_CHECK_TYPE(int64_t)
        if test "x$ac_cv_type_int64_t" = "xyes"; then
            AC_DEFINE([SDL_HAS_64BIT_TYPE], [1], [Define if supporting 64bit type])
        fi
        have_inttypes=yes
    fi
    if test "x$ac_cv_func_memcmp_working" = "xyes"; then
        AC_DEFINE([HAVE_MEMCMP],[1],[Define if supporting MEMCMP])
    fi
    if test "x$ac_cv_func_strtod" = "xyes"; then
        AC_DEFINE([HAVE_STRTOD],[1],[Define if supporting STRTOD])
    fi
fi
if test "x$have_inttypes" != "xyes"; then
    AC_CHECK_SIZEOF(char, 1)
    AC_CHECK_SIZEOF(short, 2)
    AC_CHECK_SIZEOF(int, 4)
    AC_CHECK_SIZEOF(long, 4)
    AC_CHECK_SIZEOF(long long, 8)
    if test "x$ac_cv_sizeof_char" = "x1"; then
        AC_DEFINE([int8_t], [signed char], [Define if int8_t])
        AC_DEFINE([uint8_t], [unsigned char], [Define if uint8_t])
    fi
    if test "x$ac_cv_sizeof_short" = "x2"; then
        AC_DEFINE([int16_t], [signed short], [Define if int16_t])
        AC_DEFINE([uint16_t], [unsigned short], [Define if uint16_t])
    else
        if test "x$ac_cv_sizeof_int" = "x2"; then
            AC_DEFINE([int16_t], [signed int], [Define if int16_t])
            AC_DEFINE([uint16_t], [unsigned int], [Define if uint16_t])
        fi
    fi
    if test "x$ac_cv_sizeof_int" = "x4"; then
        AC_DEFINE([int32_t], [signed int], [Define if int32_t])
        AC_DEFINE([uint32_t], [unsigned int], [Define if uint32_t])
    else
        if test "x$ac_cv_sizeof_long" = "x4"; then
            AC_DEFINE([int32_t], [signed long], [Define if int32_t])
            AC_DEFINE([uint32_t], [unsigned long], [Define if uint32_t])
        fi
    fi
    if test "x$ac_cv_sizeof_long" = "x8"; then
        AC_DEFINE([int64_t], [signed long], [Define if int64_t])
        AC_DEFINE([uint64_t], [unsigned long], [Define if uint64_t])
        AC_DEFINE([SDL_HAS_64BIT_TYPE], [1], [Define if supporting 64bit type])
    else
        if test "x$ac_cv_sizeof_long_long" = "x8"; then
            AC_DEFINE([int64_t], [signed long long], [Define if int64_t])
            AC_DEFINE([uint64_t], [unsigned long long], [Define if uint64_t])
            AC_DEFINE([SDL_HAS_64BIT_TYPE], [1], [Define if supporting 64bit type])
        fi
    fi
    AC_DEFINE([size_t], [unsigned int], [Define if size_t])
    AC_DEFINE([uintptr_t], [unsigned long], [Define if uintptr_t])
fi

# Standard C sources
SOURCES="$SOURCES $srcdir/src/*.c"
SOURCES="$SOURCES $srcdir/src/audio/*.c"
SOURCES="$SOURCES $srcdir/src/cdrom/*.c"
SOURCES="$SOURCES $srcdir/src/cpuinfo/*.c"
SOURCES="$SOURCES $srcdir/src/events/*.c"
SOURCES="$SOURCES $srcdir/src/file/*.c"
SOURCES="$SOURCES $srcdir/src/power/*.c"
SOURCES="$SOURCES $srcdir/src/stdlib/*.c"
SOURCES="$SOURCES $srcdir/src/thread/*.c"
SOURCES="$SOURCES $srcdir/src/timer/*.c"
SOURCES="$SOURCES $srcdir/src/video/*.c"

if test x$enable_audio != xyes; then
    AC_DEFINE(SDL_AUDIO_DISABLED)
fi
if test x$enable_video != xyes; then
    AC_DEFINE(SDL_VIDEO_DISABLED)
fi
if test x$enable_events != xyes; then
    AC_DEFINE(SDL_EVENTS_DISABLED)
fi
if test x$enable_joystick != xyes; then
    AC_DEFINE(SDL_JOYSTICK_DISABLED)
else
    SOURCES="$SOURCES $srcdir/src/joystick/*.c"
fi
if test x$enable_haptic != xyes; then
    AC_DEFINE(SDL_HAPTIC_DISABLED)
else
    SOURCES="$SOURCES $srcdir/src/haptic/*.c"
fi
if test x$enable_cdrom != xyes; then
    AC_DEFINE(SDL_CDROM_DISABLED)
fi
if test x$enable_threads != xyes; then
    AC_DEFINE(SDL_THREADS_DISABLED)
fi
if test x$enable_timers != xyes; then
    AC_DEFINE(SDL_TIMERS_DISABLED)
fi
if test x$enable_file != xyes; then
    AC_DEFINE(SDL_FILE_DISABLED)
fi
if test x$enable_filesystem != xyes; then
    AC_DEFINE(SDL_FILESYSTEM_DISABLED)
    else
    AC_DEFINE(SDL_FILESYSTEM_UNIX)
    SOURCES="$SOURCES $srcdir/src/filesystem/unix/*.c"
fi
if test x$enable_loadso != xyes; then
    AC_DEFINE(SDL_LOADSO_DISABLED)
fi
if test x$enable_power != xyes; then
    AC_DEFINE(SDL_POWER_DISABLED)
    else 
    AC_DEFINE(SDL_POWER_LINUX)
    SOURCES="$SOURCES $srcdir/src/power/linux/*.c"
fi
if test x$enable_cpuinfo != xyes; then
    AC_DEFINE(SDL_CPUINFO_DISABLED)
fi
if test x$enable_screensaver = xno; then
    AC_DEFINE(SDL_VIDEO_DISABLE_SCREENSAVER)
fi
dnl See if the OSS audio interface is supported
if test x$enable_audio = xyes -a x$enable_oss = xyes; then
    AC_MSG_CHECKING(for OSS audio support)
    have_oss=no
    if test x$have_oss != xyes; then
        AC_TRY_COMPILE([
            #include <sys/soundcard.h>
        ],[
            int arg = SNDCTL_DSP_SETFRAGMENT;
        ],[
        have_oss=yes
        ])
    fi
    if test x$have_oss != xyes; then
        AC_TRY_COMPILE([
            #include <soundcard.h>
        ],[
            int arg = SNDCTL_DSP_SETFRAGMENT;
        ],[
        have_oss=yes
        AC_DEFINE(SDL_AUDIO_DRIVER_OSS_SOUNDCARD_H)
        ])
    fi
    AC_MSG_RESULT($have_oss)
    if test x$have_oss = xyes; then
        AC_DEFINE(SDL_AUDIO_DRIVER_OSS)
        SOURCES="$SOURCES $srcdir/src/audio/oss/*.c"
        have_audio=yes
    fi
fi
dnl See if the ALSA audio interface is supported
if test x$enable_audio = xyes -a x$enable_alsa = xyes; then
    AM_PATH_ALSA(0.9.0, have_alsa=yes, have_alsa=no)
    # Restore all flags from before the ALSA detection runs
    CFLAGS="$alsa_save_CFLAGS"
    LDFLAGS="$alsa_save_LDFLAGS"
    LIBS="$alsa_save_LIBS"
    if test x$have_alsa = xyes; then
        alsa_lib=[`find_lib "libasound.so.*" "$ALSA_LIBS" | sed 's/.*\/\(.*\)/\1/; q'`]
        AC_DEFINE(SDL_AUDIO_DRIVER_ALSA)
        SOURCES="$SOURCES $srcdir/src/audio/alsa/*.c"
        EXTRA_CFLAGS="$EXTRA_CFLAGS $ALSA_CFLAGS"
        if test x$have_loadso != xyes && \
            test x$enable_alsa_shared = xyes; then
            AC_MSG_WARN([You must have SDL_LoadObject() support for dynamic ALSA loading])
        fi
        if test x$have_loadso = xyes && \
            test x$enable_alsa_shared = xyes && test x$alsa_lib != x; then
            echo "-- dynamic libasound -> $alsa_lib"
            AC_DEFINE_UNQUOTED(SDL_AUDIO_DRIVER_ALSA_DYNAMIC, "$alsa_lib")
        else
            EXTRA_LDFLAGS="$EXTRA_LDFLAGS $ALSA_LIBS"
        fi
        have_audio=yes
    fi
fi
dnl Find PulseAudio
if test x$enable_audio = xyes -a x$enable_pulseaudio = xyes; then
    audio_pulse=no

    PULSE_REQUIRED_VERSION=0.9

    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
    AC_MSG_CHECKING(for PulseAudio $PULSE_REQUIRED_VERSION support)
    if test x$PKG_CONFIG != xno; then
        if $PKG_CONFIG --atleast-pkgconfig-version 0.7 && $PKG_CONFIG --atleast-version $PULSE_REQUIRED_VERSION libpulse-simple; then
            PULSE_CFLAGS=`$PKG_CONFIG --cflags libpulse-simple`
            PULSE_LIBS=`$PKG_CONFIG --libs libpulse-simple`
            audio_pulse=yes
        fi
    fi
    AC_MSG_RESULT($audio_pulse)

    if test x$audio_pulse = xyes; then
        pulse_lib=[`find_lib "libpulse-simple.so.*" "$PULSE_LIBS" | sed 's/.*\/\(.*\)/\1/; q'`]

        AC_DEFINE(SDL_AUDIO_DRIVER_PULSE)
        SOURCES="$SOURCES $srcdir/src/audio/pulse/*.c"
        EXTRA_CFLAGS="$EXTRA_CFLAGS $PULSE_CFLAGS"
        if test x$have_loadso != xyes && \
            test x$enable_pulseaudio_shared = xyes; then
            AC_MSG_WARN([You must have SDL_LoadObject() support for dynamic PulseAudio loading])
        fi
        if test x$have_loadso = xyes && \
            test x$enable_pulseaudio_shared = xyes && test x$pulse_lib != x; then
            echo "-- dynamic libpulse-simple -> $pulse_lib"
            AC_DEFINE_UNQUOTED(SDL_AUDIO_DRIVER_PULSE_DYNAMIC, "$pulse_lib")

        else
            EXTRA_LDFLAGS="$EXTRA_LDFLAGS $PULSE_LIBS"
        fi
        have_audio=yes
    fi
fi
dnl See if the user wants the dummy audio driver...
if test x$enable_audio = xyes -a x$enable_dummyaudio = xyes; then
    AC_DEFINE(SDL_AUDIO_DRIVER_DUMMY)
    SOURCES="$SOURCES $srcdir/src/audio/dummy/*.c"
fi
dnl Check for ARM instruction support using gas syntax
if test x$enable_video = xyes -a x$enable_arm_simd = xyes; then
    save_CFLAGS="$CFLAGS"
    have_arm_simd=no
    CFLAGS="-x assembler-with-cpp $CFLAGS"
    
    AC_MSG_CHECKING(for ARM SIMD)
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
    .text
    .arch armv6
    .object_arch armv4
    .arm
    .altmacro
    #ifndef __ARM_EABI__
    #error EABI is required (to be sure that calling conventions are compatible)
    #endif
    pld [r0]
    uqadd8 r0, r0, r0
    ]])], have_arm_simd=yes)
    AC_MSG_RESULT($have_arm_simd)
    
    CFLAGS="$save_CFLAGS"
    
    if test x$have_arm_simd = xyes; then
        AC_DEFINE(__ARM__)
        AC_DEFINE(SDL_ARM_SIMD_BLITTERS)
        SOURCES="$SOURCES $srcdir/src/video/arm/pixman-arm-simd*.S"
    fi
fi
dnl Check for ARM NEON instruction support using gas syntax
if test x$enable_video = xyes -a x$enable_arm_neon = xyes; then
    save_CFLAGS="$CFLAGS"
    have_arm_neon=no
    CFLAGS="-x assembler-with-cpp $CFLAGS"
    
    AC_MSG_CHECKING(for ARM NEON)
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
    .text
    .fpu neon
    .arch armv7a
    .object_arch armv4
    .eabi_attribute 10, 0
    .arm
    .altmacro
    #ifndef __ARM_EABI__
    #error EABI is required (to be sure that calling conventions are compatible)
    #endif
    pld [r0]
    vmovn.u16 d0, q0
    ]])], have_arm_neon=yes)
    AC_MSG_RESULT($have_arm_neon)
    
    CFLAGS="$save_CFLAGS"
    
    if test x$have_arm_neon = xyes; then
        AC_DEFINE(__ARM__)
        AC_DEFINE(__ARM_NEON__)
        AC_DEFINE(SDL_ARM_NEON_BLITTERS)
        SOURCES="$SOURCES $srcdir/src/video/arm/pixman-arm-neon*.S"
        SOURCES="$SOURCES $srcdir/src/video/arm/notaz-arm-neon*.S"
    fi
fi
dnl Find the framebuffer console includes
if test x$enable_video = xyes -a x$enable_video_fbcon = xyes; then
    AC_MSG_CHECKING(for framebuffer console support)
    video_fbcon=no
    AC_TRY_COMPILE([
        #include <linux/fb.h>
        #include <linux/kd.h>
        #include <linux/keyboard.h>
    ],[ ],[ video_fbcon=yes ])
    AC_MSG_RESULT($video_fbcon)
    if test x$video_fbcon = xyes; then
        AC_DEFINE(SDL_VIDEO_DRIVER_FBCON)
        SOURCES="$SOURCES $srcdir/src/video/fbcon/*.c"
        have_video=yes
    fi
#    AC_CHECK_HEADER(GL/osmesa.h, have_osmesa_hdr=yes)
#    if test x$have_osmesa_hdr = xyes; then
#        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lOSMesa"
#        AC_DEFINE(HAVE_OSMESA)
#    fi
fi
dnl Find the KMSDRM libraries
if test x$enable_video = xyes -a x$enable_video_kmsdrm = xyes; then
    video_kmsdrm=no
    LIBDRM_REQUIRED_VERSION=2.4.46
    LIBUDEV_REQUIRED_VERSION=1.6.3
    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
    AC_MSG_CHECKING(for kms/drm support)
    if test x$PKG_CONFIG != xno; then
        if $PKG_CONFIG --atleast-version $LIBDRM_REQUIRED_VERSION libdrm && 
            $PKG_CONFIG --atleast-version $LIBUDEV_REQUIRED_VERSION libudev; then
            LIBDRM_CFLAGS=`$PKG_CONFIG --cflags libdrm libudev`
            LIBDRM_LIBS=`$PKG_CONFIG --libs libdrm libudev`
            video_kmsdrm=yes
        fi
    fi
    AC_MSG_RESULT($video_kmsdrm)
    if test x$video_kmsdrm = xyes; then
        if test x$kmsdrm_debug = xyes; then
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DENABLE_KMSDRM_DEBUG"
        fi
        AC_DEFINE(SDL_VIDEO_DRIVER_KMSDRM)
        SOURCES="$SOURCES $srcdir/src/video/kmsdrm/*.c"
        EXTRA_CFLAGS="$EXTRA_CFLAGS $LIBDRM_CFLAGS"
        EXTRA_LDFLAGS="$EXTRA_LDFLAGS $LIBDRM_LIBS"
        have_video=yes
    fi
fi


dnl Set up the Null video driver.
if test x$enable_video_dummy = xyes; then
    AC_DEFINE(SDL_VIDEO_DRIVER_DUMMY)
    SOURCES="$SOURCES $srcdir/src/video/dummy/*.c"
    have_video=yes
fi
dnl See if we can use the Touchscreen input library
if test x$enable_input_tslib = xyes; then
    AC_MSG_CHECKING(for Touchscreen library support)
    enable_input_tslib=no
    AC_TRY_COMPILE([
        #include "tslib.h"
    ],[ ],[
        enable_input_tslib=yes
    ])
    AC_MSG_RESULT($enable_input_tslib)
    if test x$enable_input_tslib = xyes; then
        AC_DEFINE(SDL_INPUT_TSLIB)
        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lts"
    fi
fi
dnl See if we can use GNU pth library for threads
if test x$enable_threads = xyes -a x$enable_pth = xyes; then
    AC_PATH_PROG(PTH_CONFIG, pth-config, no)
    if test "$PTH_CONFIG" = "no"; then
        use_pth=no
    else
        use_pth=yes
    fi
    AC_MSG_CHECKING(pth)
    AC_MSG_RESULT($use_pth)
    if test "x$use_pth" = xyes; then
        AC_DEFINE(SDL_THREAD_PTH)
        SOURCES="$SOURCES $srcdir/src/thread/pth/*.c"
        SOURCES="$SOURCES $srcdir/src/thread/generic/SDL_syssem.c"
        SDL_CFLAGS="$SDL_CFLAGS `$PTH_CONFIG --cflags`"
        SDL_LIBS="$SDL_LIBS `$PTH_CONFIG --libs --all`"
        have_threads=yes
    fi
fi
dnl Check for pthread support
pthread_cflags="-D_REENTRANT"
pthread_lib="-lpthread"
if test x$enable_threads = xyes -a x$enable_pthreads = xyes -a x$enable_ipod != xyes; then
    # Save the original compiler flags and libraries
    ac_save_cflags="$CFLAGS"; ac_save_libs="$LIBS"
    # Add the pthread compiler flags and libraries
    CFLAGS="$CFLAGS $pthread_cflags"; LIBS="$LIBS $pthread_lib"
    # Check to see if we have pthread support on this system
    AC_MSG_CHECKING(for pthreads)
    use_pthreads=no
    AC_TRY_LINK([
        #include <pthread.h>
    ],[
        pthread_attr_t type;
        pthread_attr_init(&type);
    ],[
        use_pthreads=yes
    ])
    AC_MSG_RESULT($use_pthreads)
    # Restore the compiler flags and libraries
    CFLAGS="$ac_save_cflags"; LIBS="$ac_save_libs"
    # Do futher testing if we have pthread support...
    if test x$use_pthreads = xyes; then
        AC_DEFINE(SDL_THREAD_PTHREAD)
        EXTRA_CFLAGS="$EXTRA_CFLAGS $pthread_cflags"
        EXTRA_LDFLAGS="$EXTRA_LDFLAGS $pthread_lib"
        SDL_CFLAGS="$SDL_CFLAGS $pthread_cflags"
        SDL_LIBS="$SDL_LIBS $pthread_lib"
        # Save the original compiler flags and libraries
        ac_save_cflags="$CFLAGS"; ac_save_libs="$LIBS"
        # Add the pthread compiler flags and libraries
        CFLAGS="$CFLAGS $pthread_cflags"; LIBS="$LIBS $pthread_lib"
        # Check to see if recursive mutexes are available
        AC_MSG_CHECKING(for recursive mutexes)
        has_recursive_mutexes=no
        if test x$has_recursive_mutexes = xno; then
            AC_TRY_LINK([
                #define _GNU_SOURCE 1
                #include <pthread.h>
            ],[
                pthread_mutexattr_t attr;
                pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            ],[
            has_recursive_mutexes=yes
            AC_DEFINE(SDL_THREAD_PTHREAD_RECURSIVE_MUTEX)
            ])
        fi
        if test x$has_recursive_mutexes = xno; then
            AC_TRY_LINK([
                #define _GNU_SOURCE 1
                #include <pthread.h>
            ],[
                pthread_mutexattr_t attr;
                pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
            ],[
            has_recursive_mutexes=yes
            AC_DEFINE(SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP)
            ])
        fi
        AC_MSG_RESULT($has_recursive_mutexes)
        # Check to see if pthread semaphore support is missing
        if test x$enable_pthread_sem = xyes; then
            AC_MSG_CHECKING(for pthread semaphores)
            have_pthread_sem=no
            AC_TRY_COMPILE([
                #include <pthread.h>
                #include <semaphore.h>
            ],[
            ],[
                have_pthread_sem=yes
            ])
            AC_MSG_RESULT($have_pthread_sem)
        fi
        if test x$have_pthread_sem = xyes; then
            AC_MSG_CHECKING(for sem_timedwait)
            have_sem_timedwait=no
            AC_TRY_LINK([
                #include <pthread.h>
                #include <semaphore.h>
            ],[
                sem_timedwait(NULL, NULL);
            ],[
                have_sem_timedwait=yes
                AC_DEFINE(HAVE_SEM_TIMEDWAIT)
            ])
            AC_MSG_RESULT($have_sem_timedwait)
        fi
        # Restore the compiler flags and libraries
        CFLAGS="$ac_save_cflags"; LIBS="$ac_save_libs"
        # Basic thread creation functions
        SOURCES="$SOURCES $srcdir/src/thread/pthread/SDL_systhread.c"
        # Semaphores
        # We can fake these with mutexes and condition variables if necessary
        if test x$have_pthread_sem = xyes; then
            SOURCES="$SOURCES $srcdir/src/thread/pthread/SDL_syssem.c"
        else
            SOURCES="$SOURCES $srcdir/src/thread/generic/SDL_syssem.c"
        fi
        # Mutexes
        # We can fake these with semaphores if necessary
        SOURCES="$SOURCES $srcdir/src/thread/pthread/SDL_sysmutex.c"
        # Condition variables
        # We can fake these with semaphores and mutexes if necessary
        SOURCES="$SOURCES $srcdir/src/thread/pthread/SDL_syscond.c"
        have_threads=yes
    else
        CheckPTH
    fi
fi
dnl Check for the dlfcn.h interface for dynamically loading objects
if test x$enable_sdl_dlopen = xyes; then
    AC_MSG_CHECKING(for dlopen)
    have_dlopen=no
    AC_TRY_COMPILE([
        #include <dlfcn.h>
    ],[
        #if defined(MAC_OS_X_VERSION_MIN_REQUIRED) && MAC_OS_X_VERSION_MIN_REQUIRED <= 1020
        #error Use dlcompat for Mac OS X 10.2 compatibility
        #endif
    ],[
        have_dlopen=yes
    ])
    AC_MSG_RESULT($have_dlopen)

    if test x$have_dlopen = xyes; then
        AC_CHECK_LIB(c, dlopen, EXTRA_LDFLAGS="$EXTRA_LDFLAGS",
            AC_CHECK_LIB(dl, dlopen, EXTRA_LDFLAGS="$EXTRA_LDFLAGS -ldl",
                AC_CHECK_LIB(ltdl, dlopen, EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lltdl")))
        AC_DEFINE(SDL_LOADSO_DLOPEN)
        SOURCES="$SOURCES $srcdir/src/loadso/dlopen/*.c"
        have_loadso=yes
    fi
fi
dnl Check for clock_gettime()
if test x$enable_clock_gettime = xyes; then
    AC_CHECK_LIB(rt, clock_gettime, have_clock_gettime=yes)
    if test x$have_clock_gettime = xyes; then
        AC_DEFINE(HAVE_CLOCK_GETTIME)
        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lrt"
    fi
fi
dnl See if GCC's -fvisibility=hidden is supported (gcc4 and later).
dnl  Details of this flag are here: http://gcc.gnu.org/wiki/Visibility
CheckVisibilityHidden() {
    AC_MSG_CHECKING(for GCC -fvisibility=hidden option)
    have_gcc_fvisibility=no

    visibility_CFLAGS="-fvisibility=hidden"
    save_CFLAGS="$CFLAGS"
    CFLAGS="$save_CFLAGS $visibility_CFLAGS -Werror"
    AC_TRY_COMPILE([
    #if !defined(__GNUC__) || __GNUC__ < 4
    #error SDL only uses visibility attributes in GCC 4 or newer
    #endif
    ],[
    ],[
    have_gcc_fvisibility=yes
    ])
    AC_MSG_RESULT($have_gcc_fvisibility)
    CFLAGS="$save_CFLAGS"

    if test x$have_gcc_fvisibility = xyes; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS $visibility_CFLAGS"
    fi
}
dnl See if GCC's -Wall is supported.
CheckWarnAll() {
    AC_MSG_CHECKING(for GCC -Wall option)
    have_gcc_Wall=no

    save_CFLAGS="$CFLAGS"
    CFLAGS="$save_CFLAGS -Wall"
    AC_TRY_COMPILE([
        int x = 0;
    ],[ ],[ have_gcc_Wall=yes ])
    AC_MSG_RESULT($have_gcc_Wall)
    CFLAGS="$save_CFLAGS"

    if test x$have_gcc_Wall = xyes; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS -Wall"
        dnl Haiku headers use multicharacter constants all over the place. Ignore these warnings when using -Wall.
        AC_MSG_CHECKING(for necessary GCC -Wno-multichar option)
        need_gcc_Wno_multichar=no
        AC_MSG_RESULT($need_gcc_Wno_multichar)
        if test x$need_gcc_Wno_multichar = xyes; then
            EXTRA_CFLAGS="$EXTRA_CFLAGS -Wno-multichar"
        fi
    fi
}
dnl Set up the configuration based on the host platform!
case "$host" in
    *-*-linux*|*-*-uclinux*|*-*-gnu*)
        ARCH=linux
        # Set up files for the joystick library
        if test x$enable_joystick = xyes; then
          case $ARCH in
            linux)
                AC_DEFINE(SDL_JOYSTICK_LINUX)
                SOURCES="$SOURCES $srcdir/src/joystick/linux/*.c"
                have_joystick=yes
            ;;
          esac
        fi
        # Set up files for the haptic library
        if test x$enable_haptic = xyes; then
            have_haptic=no
        fi
        # Set up files for the cdrom library
        if test x$enable_cdrom = xyes; then
            AC_DEFINE(SDL_CDROM_LINUX)
            SOURCES="$SOURCES $srcdir/src/cdrom/linux/*.c"
            have_cdrom=yes
        fi
        # Set up files for the timer library
        if test x$enable_timers = xyes; then
            AC_DEFINE(SDL_TIMER_UNIX)
            SOURCES="$SOURCES $srcdir/src/timer/unix/*.c"
            have_timers=yes
        fi
        ;;
    *)
        AC_MSG_ERROR([*** Unsupported host:  Please add to configure.in])
        ;;
esac
dnl Do this on all platforms, after everything else.
CheckVisibilityHidden
CheckWarnAll
# Verify that we have all the platform specific files we need
if test x$enable_joystick = xyes; then
    if test x$have_joystick != xyes; then
        # Wants joystick subsystem, but doesn't have a platform-specific backend...
        AC_DEFINE(SDL_JOYSTICK_DUMMY)
        SOURCES="$SOURCES $srcdir/src/joystick/dummy/*.c"
    fi
fi
if test x$enable_haptic = xyes; then
    if test x$have_haptic != xyes; then
        # Wants haptic subsystem, but doesn't have a platform-specific backend...
        AC_DEFINE(SDL_HAPTIC_DUMMY)
        SOURCES="$SOURCES $srcdir/src/haptic/dummy/*.c"
    fi
fi
if test x$have_cdrom != xyes; then
    if test x$enable_cdrom = xyes; then
        AC_DEFINE(SDL_CDROM_DISABLED)
    fi
    SOURCES="$SOURCES $srcdir/src/cdrom/dummy/*.c"
fi
if test x$have_threads != xyes; then
    if test x$enable_threads = xyes; then
        AC_DEFINE(SDL_THREADS_DISABLED)
    fi
    SOURCES="$SOURCES $srcdir/src/thread/generic/*.c"
fi
if test x$have_timers != xyes; then
    if test x$enable_timers = xyes; then
        AC_DEFINE(SDL_TIMERS_DISABLED)
    fi
    SOURCES="$SOURCES $srcdir/src/timer/dummy/*.c"
fi
if test x$have_loadso != xyes; then
    if test x$enable_loadso = xyes; then
        AC_DEFINE(SDL_LOADSO_DISABLED)
    fi
    SOURCES="$SOURCES $srcdir/src/loadso/dummy/*.c"
fi
if test x$SDLMAIN_SOURCES = x; then
    SDLMAIN_SOURCES="$srcdir/src/main/dummy/*.c"
    SDLMAIN_LDFLAGS="-static"
fi

OBJECTS=`echo $SOURCES | sed 's,[[^ ]]*/\([[^ ]]*\)\.asm,$(objects)/\1.lo,g'`
OBJECTS=`echo $OBJECTS | sed 's,[[^ ]]*/\([[^ ]]*\)\.cc,$(objects)/\1.lo,g'`
OBJECTS=`echo $OBJECTS | sed 's,[[^ ]]*/\([[^ ]]*\)\.m,$(objects)/\1.lo,g'`
OBJECTS=`echo $OBJECTS | sed 's,[[^ ]]*/\([[^ ]]*\)\.c,$(objects)/\1.lo,g'`
OBJECTS=`echo $OBJECTS | sed 's,[[^ ]]*/\([[^ ]]*\)\.S,$(objects)/\1.lo,g'`
OBJECTS=`echo $OBJECTS | sed 's,[[^ ]]*/\([[^ ]]*\)\.rc,$(objects)/\1.lo,g'`

SDLMAIN_OBJECTS=`echo $SDLMAIN_SOURCES | sed 's,[[^ ]]*/\([[^ ]]*\)\.cc,$(objects)/\1.lo,g'`
SDLMAIN_OBJECTS=`echo $SDLMAIN_OBJECTS | sed 's,[[^ ]]*/\([[^ ]]*\)\.m,$(objects)/\1.lo,g'`
SDLMAIN_OBJECTS=`echo $SDLMAIN_OBJECTS | sed 's,[[^ ]]*/\([[^ ]]*\)\.c,$(objects)/\1.lo,g'`

# Set runtime shared library paths as needed
if test "x$enable_rpath" = "xyes"; then
    SDL_RLD_FLAGS="-Wl,-rpath,\${libdir}"
else
    SDL_RLD_FLAGS=""
fi
    SDL_STATIC_LIBS="$SDL_LIBS $EXTRA_LDFLAGS"

dnl Expand the cflags and libraries needed by apps using SDL
AC_SUBST(SDL_CFLAGS)
AC_SUBST(SDL_LIBS)
AC_SUBST(SDL_STATIC_LIBS)
AC_SUBST(SDL_RLD_FLAGS)
if test x$enable_shared = xyes; then
    ENABLE_SHARED_TRUE=
    ENABLE_SHARED_FALSE="#"
else
    ENABLE_SHARED_TRUE="#"
    ENABLE_SHARED_FALSE=
fi
if test x$enable_static = xyes; then
    ENABLE_STATIC_TRUE=
    ENABLE_STATIC_FALSE="#"
else
    ENABLE_STATIC_TRUE="#"
    ENABLE_STATIC_FALSE=
fi
AC_SUBST(ENABLE_SHARED_TRUE)
AC_SUBST(ENABLE_SHARED_FALSE)
AC_SUBST(ENABLE_STATIC_TRUE)
AC_SUBST(ENABLE_STATIC_FALSE)

dnl Expand the sources and objects needed to build the library
AC_SUBST(ac_aux_dir)
AC_SUBST(INCLUDE)
AC_SUBST(SOURCES)
AC_SUBST(OBJECTS)
AC_SUBST(SDLMAIN_SOURCES)
AC_SUBST(SDLMAIN_OBJECTS)
AC_SUBST(SDLMAIN_LDFLAGS)
AC_SUBST(BUILD_CFLAGS)
AC_SUBST(EXTRA_CFLAGS)
AC_SUBST(BUILD_LDFLAGS)
AC_SUBST(EXTRA_LDFLAGS)

AC_OUTPUT([
    Makefile sdl-config sdl.pc
], [
    : >build-deps
    if test x"$MAKE" = x;
        then MAKE=make;
    fi;
    $MAKE depend
])
