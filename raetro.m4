dnl ###################################################################################
dnl  Group         : Build Systems
dnl  Project       : TinyRetroLabs
dnl  Module        : Automake Macros
dnl  Purpose       : Provide Easy Macros for Common Required Tasks.
dnl  License       : GNU General Public License v3.0
dnl ###################################################################################
dnl 
dnl  SYNOPSIS
dnl 
dnl    TRL_FIND_LIBRARY(FLAG, [ACTION-SUCCESS], [ACTION-FAILURE], [EXTRA-FLAGS])
dnl    TRL_SETUP_ARCH_DEFINES(FLAG, [ACTION-SUCCESS], [ACTION-FAILURE], [EXTRA-FLAGS])
dnl    ERROR_IF_NO_PROG(FLAG, [ACTION-SUCCESS], [ACTION-FAILURE], [EXTRA-FLAGS])
dnl    AX_APPEND_FLAG(FLAG, [FLAGS-VARIABLE])
dnl    AX_APPEND_COMPILE_FLAGS([FLAG1 FLAG2 ...], [FLAGS-VARIABLE], [EXTRA-FLAGS])
dnl    AX_APPEND_LINK_FLAGS([FLAG1 FLAG2 ...], [FLAGS-VARIABLE], [EXTRA-FLAGS])
dnl 
dnl ###################################################################################
dnl 
dnl  Copyright (c) 2019-2020 Marcus Andrade
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
AC_DEFUN([TRL_FIND_LIBRARY], [
    AC_MSG_CHECKING([for lib$2 soname])
    $1_FILENAME=$($CC -nostdlib -o /dev/null $LDFLAGS -l$2 -Wl,-M 2>/dev/null | grep "^LOAD.*$2" | awk '{V=2; print $V}')
    if [[ -z $$1_FILENAME ]]; then
        #try gold linker syntax
        $1_FILENAME=$($CC -nostdlib -o /dev/null $LDFLAGS -l$2 -Wl,-t 3>&1 1>&2 2>&3 | grep "lib$2")
    fi
    if [[ ! -z $$1_FILENAME ]]; then
        $1_SONAME=$($OBJDUMP -p $$1_FILENAME | grep "SONAME.*$2" | awk '{V=2; print $V}')
    fi
    if [[ -z "$$1_SONAME" ]]; then
    AC_MSG_RESULT([no])
        if test -z "$3" || test "x${$3}" = "xyes"; then
            AC_MSG_ERROR([Unable to determine soname of lib$2 library])
        else
            AC_MSG_WARN([Unable to determine soname of lib$2 library])
            $3=no
            AC_MSG_WARN([lib$2 support disabled])
        fi
    else
        AC_MSG_RESULT([$$1_SONAME])
        AC_SUBST($1_SONAME)
    fi
])
dnl ###################################################################################
AC_DEFUN([AS_TINY_ARCH_DETECTION],[
    AC_MSG_RESULT([------------------------->>>>>    HOST AND PLATFORM SETTINGS    <<<<<-------------------------])
    # host detection and setup
    case $host in
    arm*-*-linux*|arm*-*-uclinux*)
        AC_DEFINE([__ARM__],[1],[Define if supporting ARM CPU])
        AC_SUBST(ARCH_DEFINES, "-DTARGET_POSIX -DTARGET_LINUX -D_LINUX -D_ARM -DTARGET_ARM")
        ;;
    mips*-*-linux*|mips*-*-uclinux*)
        AC_DEFINE([__MIPS__],[1],[Define if supporting MIPS CPU])
        AC_SUBST(ARCH_DEFINES, "-DTARGET_POSIX -DTARGET_LINUX -D_LINUX -D_MIPS -DTARGET_MIPS")
        ;;
    *-*-linux*|*-*-uclinux*)
        AC_SUBST(ARCH_DEFINES, "-DTARGET_POSIX -DTARGET_LINUX -D_LINUX")
        ;;
    *)
        AC_MSG_ERROR(Unsupported host ($host))
    esac

    case $use_platform in
    mister)
        ARCH="armv7-a"
        use_fpu="neon"
        use_arch="arm"
        use_cpu="cortex-a9"
        use_tune="cortex-a9"
        use_joystick="yes"
        use_abi="hard"
        use_neon="yes"
        enable_shared="yes"
        enable_static="no"
        AC_SUBST(ARCH_DEFINES, "-DTARGET_MISTER")
        ;;
    raspberry-pi)
        AC_SUBST(ARCH_DEFINES, "-D_ARMEL -DTARGET_RASPBERRY_PI")
        ;;
    esac
    AC_SUBST([ARCH])
])
dnl ###################################################################################
dnl ERROR_IF_NO_PROG
dnl   A quick / dirty macro to ensure that a required program / executable
dnl   is on PATH. If it is not we display an error message using AC_MSG_ERROR.
dnl $1: program name
AC_DEFUN([ERROR_IF_NO_PROG],[
    AC_CHECK_PROG([result_$1], [$1], [yes], [no])
    AS_IF([test "x$result_$1" != "xyes"], [
        AC_MSG_ERROR([Missing required program '$1': ensure it is installed and on PATH.])
    ])
])
dnl ###################################################################################
dnl autostars m4 macro for detection of gcc inline assembly
dnl by David Schleef <ds@schleef.org>
dnl ###################################################################################
AC_DEFUN([AS_GCC_INLINE_ASSEMBLY], [
    AC_MSG_CHECKING([if compiler supports gcc-style inline assembly])
    AC_TRY_COMPILE([], [
        #ifdef __GNUC_MINOR__
        #if (__GNUC__ * 1000 + __GNUC_MINOR__) < 3004
        #error GCC before 3.4 has critical bugs compiling inline assembly
        #endif
        #endif
        __asm__ (""::) ],
        [flag_ok=yes],
        [flag_ok=no]
    )
    if test "x$flag_ok" = "xyes" ; then
        $1
        true
    else
        $2
        true
    fi
    AC_MSG_RESULT([$flag_ok])
])
dnl ###################################################################################
dnl Check for ARM SIMD assembly instruction support
dnl ###################################################################################
AC_DEFUN([AS_ASM_ARM_SIMD], [
    AC_MSG_CHECKING([support for ARMv6 SIMD instructions])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([],[__asm__("uqadd8 r0, r0, r0")])],
        [
            AC_DEFINE([_ARM_SIMD_],[1],[Define if supporting ARM SIMD (ARMv6)])
            AC_MSG_RESULT([yes]);
            $1
        ],
        [AC_MSG_RESULT([no]); $2]
    )
])
dnl ###################################################################################
dnl Check for ARM NEON assembly instruction support
dnl ###################################################################################
AC_DEFUN([AS_ASM_ARM_NEON], [
    AC_MSG_CHECKING([support for ARM NEON instructions])
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([],[__asm__("vmovn.u16 d0, q0")])],
        [
            AC_DEFINE([_ARM_NEON_],[1],[Define if supporting ARM NEON])
            AC_MSG_RESULT([yes]);
            $1
        ],
        [AC_MSG_RESULT([no]); $2]
    )
])
dnl ###################################################################################
AC_DEFUN([AX_CHECK_LINK_FLAG],[
    AS_VAR_PUSHDEF([CACHEVAR],[ax_cv_check_ldflags_$4_$1])
    AC_CACHE_CHECK([for linker option $1], CACHEVAR, [
        ax_check_save_flags=$LDFLAGS
        LDFLAGS="$LDFLAGS $4 $1"
        AC_LINK_IFELSE([
            AC_LANG_PROGRAM()
        ],[
            AS_VAR_SET([CACHEVAR],[yes])
            ],[
            AS_VAR_SET([CACHEVAR],[no])
        ])
        LDFLAGS=$ax_check_save_flags
    ])
    AS_VAR_IF([CACHEVAR], ["yes"], [
        m4_default([$2], :)
    ], [
        m4_default([$3], :)
    ])
    AS_VAR_POPDEF([CACHEVAR])
])

AC_DEFUN([AX_CHECK_COMPILE_FLAG],[
    AS_VAR_PUSHDEF([CACHEVAR],[ax_cv_check_[]_AC_LANG_ABBREV[]flags_$4_$1])
    AC_CACHE_CHECK([whether _AC_LANG compiler accepts $1], CACHEVAR, [
        ax_check_save_flags=$[]_AC_LANG_PREFIX[]FLAGS
        _AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS $4 $1"
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],[
            AS_VAR_SET([CACHEVAR],[yes])
        ],[
            AS_VAR_SET([CACHEVAR],[no])
        ])
        _AC_LANG_PREFIX[]FLAGS=$ax_check_save_flags
    ])
    AS_VAR_IF([CACHEVAR], ["yes"], [
        m4_default([$2], :)
    ],[
        m4_default([$3], :)
    ])
    AS_VAR_POPDEF([CACHEVAR])
])

AC_DEFUN([AX_APPEND_FLAG], [
    AC_REQUIRE([AC_PROG_GREP])
    AS_VAR_PUSHDEF([FLAGS], [m4_default($2,_AC_LANG_PREFIX[]FLAGS)])
    AS_VAR_SET_IF([FLAGS], [AS_IF([AS_ECHO(" $[]FLAGS ") | $GREP " $1 " 2>&1 >/dev/null],
    [AC_RUN_LOG([: FLAGS already contains $1])],
    [AC_RUN_LOG([: FLAGS="$FLAGS $1"])
    AS_VAR_APPEND([FLAGS], [" $1"])])],
    [AS_VAR_SET([FLAGS],[$1])])
    AS_VAR_POPDEF([FLAGS])
])

AC_DEFUN([AX_APPEND_COMPILE_FLAGS], [
    for flag in $1; do
        AX_CHECK_COMPILE_FLAG([$flag], [AX_APPEND_FLAG([$flag], [$2])], [], [$3])
    done
])

AC_DEFUN([AX_APPEND_LINK_FLAGS], [
    for flag in $1; do
        AX_CHECK_LINK_FLAG([$flag], [AX_APPEND_FLAG([$flag], [m4_default([$2], [LDFLAGS])])], [], [$3])
    done
])

AC_DEFUN([AX_APPEND_LINK_FLAGS], [
    for flag in $1; do
        AX_CHECK_LINK_FLAG([$flag], [AX_APPEND_FLAG([$flag], [m4_default([$2], [LDFLAGS])])], [], [$3])
    done
])
