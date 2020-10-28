/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2020 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "SDL_config.h"

/* CPU feature detection for SDL */

#include "SDL.h"
#include "SDL_cpuinfo.h"

#define SDL_NULL_WHILE_LOOP_CONDITION (0)

#define SDL_disabled_assert(condition) do { (void) sizeof ((condition)); } while (SDL_NULL_WHILE_LOOP_CONDITION)

#if SDL_ASSERT_LEVEL == 0   /* assertions disabled */
#   define SDL_assert(condition) SDL_disabled_assert(condition)
#   define SDL_assert_release(condition) SDL_disabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)
#elif SDL_ASSERT_LEVEL == 1  /* release settings. */
#   define SDL_assert(condition) SDL_disabled_assert(condition)
#   define SDL_assert_release(condition) SDL_enabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)
#elif SDL_ASSERT_LEVEL == 2  /* normal settings. */
#   define SDL_assert(condition) SDL_enabled_assert(condition)
#   define SDL_assert_release(condition) SDL_enabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)
#elif SDL_ASSERT_LEVEL == 3  /* paranoid settings. */
#   define SDL_assert(condition) SDL_enabled_assert(condition)
#   define SDL_assert_release(condition) SDL_enabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_enabled_assert(condition)
#else
#   error Unknown assertion level.
#endif

#ifdef HAVE_SYSCONF
#include <unistd.h>
#endif
#ifdef HAVE_SYSCTLBYNAME
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if defined(__LINUX__) && defined(__ARM__)
/*#include <asm/hwcap.h>*/
#ifndef AT_HWCAP
#define AT_HWCAP 16
#endif
#ifndef AT_PLATFORM
#define AT_PLATFORM 15
#endif
/* Prevent compilation error when including elf.h would also try to define AT_* as an enum */
#ifndef AT_NULL
#define AT_NULL 0
#endif
#ifndef HWCAP_NEON
#define HWCAP_NEON (1 << 12)
#endif
#if defined HAVE_GETAUXVAL
#include <sys/auxv.h>
#else
#include <fcntl.h>
#endif
#endif

#define CPU_HAS_RDTSC    (1 << 0)
#define CPU_HAS_ALTIVEC  (1 << 1)
#define CPU_HAS_MMX      (1 << 2)
#define CPU_HAS_MMXEXT   (1 << 3)
#define CPU_HAS_3DNOW    (1 << 4)
#define CPU_HAS_3DNOWEXT (1 << 5)
#define CPU_HAS_SSE      (1 << 6)
#define CPU_HAS_SSE2     (1 << 7)
#define CPU_HAS_SSE3     (1 << 8)
#define CPU_HAS_SSE41    (1 << 9)
#define CPU_HAS_SSE42    (1 << 10)
#define CPU_HAS_AVX      (1 << 11)
#define CPU_HAS_AVX2     (1 << 12)
#define CPU_HAS_AVX512F  (1 << 13)
#define CPU_HAS_ARM_NEON (1 << 14)
#define CPU_HAS_ARM_SIMD (1 << 15)

static int CPU_haveCPUID(void) {
	int has_CPUID = 0;

	/* *INDENT-OFF* */
#ifndef SDL_CPUINFO_DISABLED
#if defined(__GNUC__) && defined(i386)
	__asm__ (
"        pushfl                      # Get original EFLAGS             \n"
"        popl    %%eax                                                 \n"
"        movl    %%eax,%%ecx                                           \n"
"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
"        pushl   %%eax               # Save new EFLAGS value on stack  \n"
"        popfl                       # Replace current EFLAGS value    \n"
"        pushfl                      # Get new EFLAGS                  \n"
"        popl    %%eax               # Store new EFLAGS in EAX         \n"
"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
"        jz      1f                  # Processor=80486                 \n"
"        movl    $1,%0               # We have CPUID support           \n"
"1:                                                                    \n"
	: "=m" (has_CPUID)
	:
	: "%eax", "%ecx"
	);
#elif defined(__GNUC__) && defined(__x86_64__)
	/* Technically, if this is being compiled under __x86_64__ then it has
	   CPUid by definition.  But it's nice to be able to prove it.  :)      */
	__asm__ (
	"        pushfq                      # Get original EFLAGS             \n"
	"        popq    %%rax                                                 \n"
	"        movq    %%rax,%%rcx                                           \n"
	"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
	"        pushq   %%rax               # Save new EFLAGS value on stack  \n"
	"        popfq                       # Replace current EFLAGS value    \n"
	"        pushfq                      # Get new EFLAGS                  \n"
	"        popq    %%rax               # Store new EFLAGS in EAX         \n"
	"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
	"        jz      1f                  # Processor=80486                 \n"
	"        movl    $1,%0               # We have CPUID support           \n"
	"1:                                                                    \n"
	: "=m" (has_CPUID)
	:
	: "%rax", "%rcx"
	);
#endif
#endif
	/* *INDENT-ON* */
	return has_CPUID;
}

#define cpuid(func, a, b, c, d) do { a = b = c = d = 0; (void) a; (void) b; (void) c; (void) d; } while (0)

static int CPU_CPUIDFeatures[4];
static int CPU_CPUIDMaxFunction = 0;
static SDL_bool CPU_OSSavesYMM = SDL_FALSE;
static SDL_bool CPU_OSSavesZMM = SDL_FALSE;

static void CPU_calcCPUIDFeatures(void) {
	static SDL_bool checked = SDL_FALSE;
	if(!checked) {
		checked = SDL_TRUE;
		if(CPU_haveCPUID()) {
			int a, b, c, d;
			cpuid(0, a, b, c, d);
			CPU_CPUIDMaxFunction = a;
			if(CPU_CPUIDMaxFunction >= 1) {
				cpuid(1, a, b, c, d);
				CPU_CPUIDFeatures[0] = a;
				CPU_CPUIDFeatures[1] = b;
				CPU_CPUIDFeatures[2] = c;
				CPU_CPUIDFeatures[3] = d;
				if(c & 0x08000000) {
					CPU_OSSavesYMM = ((a & 6) == 6) ? SDL_TRUE : SDL_FALSE;
					CPU_OSSavesZMM = (CPU_OSSavesYMM && ((a & 0xe0) == 0xe0)) ? SDL_TRUE : SDL_FALSE;
				}
			}
		}
	}
}

static __inline__ int CPU_getCPUIDFeatures(void) {
	int features = 0;
#if defined(__GNUC__) && defined(i386)
	__asm__ (
"        xorl    %%eax,%%eax         # Set up for CPUID instruction    \n"
"        pushl   %%ebx                                                 \n"
"        cpuid                       # Get and save vendor ID          \n"
"        popl    %%ebx                                                 \n"
"        cmpl    $1,%%eax            # Make sure 1 is valid input for CPUID\n"
"        jl      1f                  # We dont have the CPUID instruction\n"
"        xorl    %%eax,%%eax                                           \n"
"        incl    %%eax                                                 \n"
"        pushl   %%ebx                                                 \n"
"        cpuid                       # Get family/model/stepping/features\n"
"        popl    %%ebx                                                 \n"
"        movl    %%edx,%0                                              \n"
"1:                                                                    \n"
	: "=m" (features)
	:
	: "%eax", "%ecx", "%edx"
	);
#elif defined(__GNUC__) && defined(__x86_64__)
	__asm__ (
	"        xorl    %%eax,%%eax         # Set up for CPUID instruction    \n"
	"        pushq   %%rbx                                                 \n"
	"        cpuid                       # Get and save vendor ID          \n"
	"        popq    %%rbx                                                 \n"
	"        cmpl    $1,%%eax            # Make sure 1 is valid input for CPUID\n"
	"        jl      1f                  # We dont have the CPUID instruction\n"
	"        xorl    %%eax,%%eax                                           \n"
	"        incl    %%eax                                                 \n"
	"        pushq   %%rbx                                                 \n"
	"        cpuid                       # Get family/model/stepping/features\n"
	"        popq    %%rbx                                                 \n"
	"        movl    %%edx,%0                                              \n"
	"1:                                                                    \n"
	: "=m" (features)
	:
	: "%rax", "%rcx", "%rdx"
	);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
	__asm {
		xor     eax, eax            ; Set up for CPUID instruction
		push    ebx
		cpuid                       ; Get and save vendor ID
		pop     ebx
		cmp     eax, 1              ; Make sure 1 is valid input for CPUID
		jl      done                ; We dont have the CPUID instruction
		xor     eax, eax
		inc     eax
		push    ebx
		cpuid                       ; Get family/model/stepping/features
		pop     ebx
		mov     features, edx
done:
	}
#elif defined(__sun) && (defined(__i386) || defined(__amd64))
		__asm(
"        xorl    %eax,%eax         \n"
"        pushl   %ebx              \n"
"        cpuid                     \n"
"        popl    %ebx              \n"
"        cmpl    $1,%eax           \n"
"        jl      1f                \n"
"        xorl    %eax,%eax         \n"
"        incl    %eax              \n"
"        pushl   %ebx              \n"
"        cpuid                     \n"
"        popl    %ebx              \n"
#ifdef __i386
"        movl    %edx,-8(%ebp)     \n"
#else
"        movl    %edx,-8(%rbp)     \n"
#endif
"1:                                \n"
		);
#endif
	return features;
}

static __inline__ int CPU_getCPUIDFeaturesExt(void) {
	int features = 0;
#if defined(__GNUC__) && defined(i386)
	__asm__ (
"        movl    $0x80000000,%%eax   # Query for extended functions    \n"
"        pushl   %%ebx                                                 \n"
"        cpuid                       # Get extended function limit     \n"
"        popl    %%ebx                                                 \n"
"        cmpl    $0x80000001,%%eax                                     \n"
"        jl      1f                  # Nope, we dont have function 800000001h\n"
"        movl    $0x80000001,%%eax   # Setup extended function 800000001h\n"
"        pushl   %%ebx                                                 \n"
"        cpuid                       # and get the information         \n"
"        popl    %%ebx                                                 \n"
"        movl    %%edx,%0                                              \n"
"1:                                                                    \n"
	: "=m" (features)
	:
	: "%eax", "%ecx", "%edx"
	);
#elif defined(__GNUC__) && defined (__x86_64__)
	__asm__ (
"        movl    $0x80000000,%%eax   # Query for extended functions    \n"
"        pushq   %%rbx                                                 \n"
"        cpuid                       # Get extended function limit     \n"
"        popq    %%rbx                                                 \n"
"        cmpl    $0x80000001,%%eax                                     \n"
"        jl      1f                  # Nope, we dont have function 800000001h\n"
"        movl    $0x80000001,%%eax   # Setup extended function 800000001h\n"
"        pushq   %%rbx                                                 \n"
"        cpuid                       # and get the information         \n"
"        popq    %%rbx                                                 \n"
"        movl    %%edx,%0                                              \n"
"1:                                                                    \n"
	: "=m" (features)
	:
	: "%rax", "%rcx", "%rdx"
	);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
	__asm {
		mov     eax,80000000h       ; Query for extended functions
		push    ebx
		cpuid                       ; Get extended function limit
		pop     ebx
		cmp     eax,80000001h
		jl      done                ; Nope, we dont have function 800000001h
		mov     eax,80000001h       ; Setup extended function 800000001h
		push    ebx
		cpuid                       ; and get the information
		pop     ebx
		mov     features,edx
done:
	}
#elif defined(__sun) && ( defined(__i386) || defined(__amd64) )
		__asm (
"        movl    $0x80000000,%eax \n"
"        pushl   %ebx             \n"
"        cpuid                    \n"
"        popl    %ebx             \n"
"        cmpl    $0x80000001,%eax \n"
"        jl      1f               \n"
"        movl    $0x80000001,%eax \n"
"        pushl   %ebx             \n"
"        cpuid                    \n"
"        popl    %ebx             \n"
#ifdef __i386
"        movl    %edx,-8(%ebp)   \n"
#else
"        movl    %edx,-8(%rbp)   \n"
#endif
"1:                               \n"
		);
#endif
	return features;
}

////////////////////////////////////////////////////////////////////////////////
// Check CPU Capabilities
////////////////////////////////////////////////////////////////////////////////
static int CPU_haveRDTSC(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeatures() & 0x00000010);
	}
	return 0;
}

static int CPU_haveMMX(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeatures() & 0x00800000);
	}
	return 0;
}

static int CPU_haveMMXExt(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeaturesExt() & 0x00400000);
	}
	return 0;
}

static int CPU_have3DNow(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeaturesExt() & 0x80000000);
	}
	return 0;
}

static int CPU_have3DNowExt(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeaturesExt() & 0x40000000);
	}
	return 0;
}

static int CPU_haveSSE(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeatures() & 0x02000000);
	}
	return 0;
}

static int CPU_haveSSE2(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeatures() & 0x04000000);
	}
	return 0;
}

static int CPU_haveSSE3(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeatures() & 0x00000001);
	}
	return 0;
}

static int CPU_haveSSE41(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeatures() & 0x00080000);
	}
	return 0;
}

static int CPU_haveSSE42(void) {
	if(CPU_haveCPUID()) {
		return (CPU_getCPUIDFeatures() & 0x00100000);
	}
	return 0;
}

static int CPU_haveAVX(void) {
	if(CPU_haveCPUID()) {
		return (CPU_OSSavesYMM && CPU_getCPUIDFeatures() & 0x10000000);
	}
	return 0;
}

static int CPU_haveAVX2(void) {
	if(CPU_OSSavesYMM && (CPU_CPUIDMaxFunction >= 7)) {
		int a, b, c, d;
		(void) a;
		(void) b;
		(void) c;
		(void) d;  /* compiler warnings... */
		cpuid(7, a, b, c, d);
		return (b & 0x00000020);
	}
	return 0;
}

static int CPU_haveAVX512F(void) {
	if(CPU_OSSavesZMM && (CPU_CPUIDMaxFunction >= 7)) {
		int a, b, c, d;
		(void) a;
		(void) b;
		(void) c;
		(void) d;  /* compiler warnings... */
		cpuid(7, a, b, c, d);
		return (b & 0x00010000);
	}
	return 0;
}

#if defined(__ARM_ARCH) && (__ARM_ARCH >= 6)
static int CPU_haveARMSIMD(void) {
	return 1;
}
#elif !defined(__ARM__)

static int CPU_haveARMSIMD(void) {
	return 0;
}

#elif defined(__LINUX__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>

static int CPU_haveARMSIMD(void) {
	int arm_simd = 0;
	int fd;

	fd = open("/proc/self/auxv", O_RDONLY);
	if(fd >= 0) {
		Elf32_auxv_t aux;
		while (read(fd, &aux, sizeof aux) == sizeof aux) {
			if(aux.a_type == AT_PLATFORM) {
				const char *plat = (const char *) aux.a_un.a_val;
				if(plat) {
					arm_simd = strncmp(plat, "v6l", 3) == 0 || strncmp(plat, "v7l", 3) == 0;
				}
			}
		}
		close(fd);
	}
	return arm_simd;
}

#else

static int CPU_haveARMSIMD(void) {
#warning SDL_HasARMSIMD is not implemented for this ARM platform. Write me.
	return 0;
}
#endif

#if defined(__LINUX__) && defined(__ARM__) && !defined(HAVE_GETAUXVAL)
static int readProcAuxvForNeon(void) {
	int neon = 0;
	int kv[2];
	const int fd = open("/proc/self/auxv", O_RDONLY);
	if (fd != -1) {
		while (read(fd, kv, sizeof (kv)) == sizeof (kv)) {
			if (kv[0] == AT_HWCAP) {
				neon = ((kv[1] & HWCAP_NEON) == HWCAP_NEON);
				break;
			}
		}
		close(fd);
	}
	return neon;
}
#endif

static int CPU_haveARMNEON(void) {
#if defined(SDL_CPUINFO_DISABLED)
	return 0; /* disabled */
#elif !defined(__ARM__)
	return 0;  /* not an ARM CPU at all. */
#elif __ARM_ARCH >= 8
	return 1;  /* ARMv8 always has non-optional NEON support. */
#elif defined(__LINUX__)
	return readProcAuxvForNeon();
#else
#warning SDL_HasNEON is not implemented for this ARM platform. Write me.
	return 0;
#endif
}

static int CPU_haveAltiVec(void) {
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// CPU Count
////////////////////////////////////////////////////////////////////////////////
static int SDL_CPUCount = 0;

int SDL_GetCPUCount(void) {
	if(!SDL_CPUCount) {
#ifndef SDL_CPUINFO_DISABLED
#if defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_ONLN)
		if (SDL_CPUCount <= 0) {
			SDL_CPUCount = (int)sysconf(_SC_NPROCESSORS_ONLN);
		}
#endif
#ifdef HAVE_SYSCTLBYNAME
		if (SDL_CPUCount <= 0) {
			size_t size = sizeof(SDL_CPUCount);
			sysctlbyname("hw.ncpu", &SDL_CPUCount, &size, NULL, 0);
		}
#endif
#endif
		/* There has to be at least 1, right? :) */
		if(SDL_CPUCount <= 0) {
			SDL_CPUCount = 1;
		}
	}
	return SDL_CPUCount;
}

/* Oh, such a sweet sweet trick, just not very useful. :) */
static const char *SDL_GetCPUType(void) {
	static char SDL_CPUType[13];

	if(!SDL_CPUType[0]) {
		int i = 0;

		CPU_calcCPUIDFeatures();
		if(CPU_CPUIDMaxFunction > 0) {  /* do we have CPUID at all? */
			int a, b, c, d;
			cpuid(0x00000000, a, b, c, d);
			(void) a;
			SDL_CPUType[i++] = (char) (b & 0xff);
			b >>= 8;
			SDL_CPUType[i++] = (char) (b & 0xff);
			b >>= 8;
			SDL_CPUType[i++] = (char) (b & 0xff);
			b >>= 8;
			SDL_CPUType[i++] = (char) (b & 0xff);

			SDL_CPUType[i++] = (char) (d & 0xff);
			d >>= 8;
			SDL_CPUType[i++] = (char) (d & 0xff);
			d >>= 8;
			SDL_CPUType[i++] = (char) (d & 0xff);
			d >>= 8;
			SDL_CPUType[i++] = (char) (d & 0xff);

			SDL_CPUType[i++] = (char) (c & 0xff);
			c >>= 8;
			SDL_CPUType[i++] = (char) (c & 0xff);
			c >>= 8;
			SDL_CPUType[i++] = (char) (c & 0xff);
			c >>= 8;
			SDL_CPUType[i++] = (char) (c & 0xff);
		}
		if(!SDL_CPUType[0]) {
			SDL_strlcpy(SDL_CPUType, "Unknown", sizeof(SDL_CPUType));
		}
	}
	return SDL_CPUType;
}

int SDL_GetCPUCacheLineSize(void) {
	const char *cpuType = SDL_GetCPUType();
	int a, b, c, d;
	(void) a;
	(void) b;
	(void) c;
	(void) d;
	if(SDL_strcmp(cpuType, "GenuineIntel") == 0) {
		cpuid(0x00000001, a, b, c, d);
		return (((b >> 8) & 0xff) * 8);
	} else if(SDL_strcmp(cpuType, "AuthenticAMD") == 0 || SDL_strcmp(cpuType, "HygonGenuine") == 0) {
		cpuid(0x80000005, a, b, c, d);
		return (c & 0xff);
	} else {
		/* Just make a guess here... */
		return SDL_CACHELINE_SIZE;
	}
}

static Uint32 SDL_CPUFeatures = 0xFFFFFFFF;
static Uint32 SDL_SIMDAlignment = 0xFFFFFFFF;

static int SDL_SystemRAM = 0;

int SDL_GetSystemRAM(void) {
	if(!SDL_SystemRAM) {
#ifndef SDL_CPUINFO_DISABLED
#if defined(HAVE_SYSCONF) && defined(_SC_PHYS_PAGES) && defined(_SC_PAGESIZE)
		if (SDL_SystemRAM <= 0) {
			SDL_SystemRAM = (int)((Sint64)sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE) / (1024*1024));
		}
#endif
#ifdef HAVE_SYSCTLBYNAME
		if (SDL_SystemRAM <= 0) {
			Uint64 memsize = 0;
			size_t len = sizeof(memsize);

			if (sysctl(mib, 2, &memsize, &len, NULL, 0) == 0) {
				SDL_SystemRAM = (int)(memsize / (1024*1024));
			}
		}
#endif
#endif
	}
	return SDL_SystemRAM;
}

static Uint32 SDL_GetCPUFeatures(void) {
	if(SDL_CPUFeatures == 0xFFFFFFFF) {
		CPU_calcCPUIDFeatures();
		SDL_CPUFeatures = 0;
		SDL_SIMDAlignment = sizeof(void *);  /* a good safe base value */
		if(CPU_haveRDTSC()) {
			SDL_CPUFeatures |= CPU_HAS_RDTSC;
		}
		if(CPU_haveAltiVec()) {
			SDL_CPUFeatures |= CPU_HAS_ALTIVEC;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
		if(CPU_haveMMX()) {
			SDL_CPUFeatures |= CPU_HAS_MMX;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 8);
		}
		if(CPU_haveMMXExt()) {
			SDL_CPUFeatures |= CPU_HAS_MMXEXT;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 8);
		}
		if(CPU_have3DNow()) {
			SDL_CPUFeatures |= CPU_HAS_3DNOW;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 8);
		}
		if(CPU_have3DNowExt()) {
			SDL_CPUFeatures |= CPU_HAS_3DNOWEXT;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 8);
		}
		if(CPU_haveSSE()) {
			SDL_CPUFeatures |= CPU_HAS_SSE;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
		if(CPU_haveSSE2()) {
			SDL_CPUFeatures |= CPU_HAS_SSE2;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
		if(CPU_haveSSE3()) {
			SDL_CPUFeatures |= CPU_HAS_SSE3;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
		if(CPU_haveSSE41()) {
			SDL_CPUFeatures |= CPU_HAS_SSE41;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
		if(CPU_haveSSE42()) {
			SDL_CPUFeatures |= CPU_HAS_SSE42;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
		if(CPU_haveAVX()) {
			SDL_CPUFeatures |= CPU_HAS_AVX;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 32);
		}
		if(CPU_haveAVX2()) {
			SDL_CPUFeatures |= CPU_HAS_AVX2;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 32);
		}
		if(CPU_haveAVX512F()) {
			SDL_CPUFeatures |= CPU_HAS_AVX512F;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 64);
		}
		if(CPU_haveARMSIMD()) {
			SDL_CPUFeatures |= CPU_HAS_ARM_SIMD;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
		if(CPU_haveARMNEON()) {
			SDL_CPUFeatures |= CPU_HAS_ARM_NEON;
			SDL_SIMDAlignment = SDL_max(SDL_SIMDAlignment, 16);
		}
	}
	return SDL_CPUFeatures;
}

#define CPU_FEATURE_AVAILABLE(f) ((SDL_GetCPUFeatures() & f) ? SDL_TRUE : SDL_FALSE)

////////////////////////////////////////////////////////////////////////////////
// CPU Count
////////////////////////////////////////////////////////////////////////////////
SDL_bool SDL_HasRDTSC(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_RDTSC);
}

SDL_bool SDL_HasMMX(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_MMX);
}

SDL_bool SDL_HasMMXExt(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_MMXEXT);
}

SDL_bool SDL_Has3DNow(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_3DNOW);
}

SDL_bool SDL_Has3DNowExt(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_3DNOWEXT);
}

SDL_bool SDL_HasSSE(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_SSE);
}

SDL_bool SDL_HasSSE2(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_SSE2);
}

SDL_bool SDL_HasSSE3(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_SSE3);
}

SDL_bool SDL_HasSSE41(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_SSE41);
}

SDL_bool SDL_HasSSE42(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_SSE42);
}

SDL_bool SDL_HasAVX(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_AVX);
}

SDL_bool SDL_HasAVX2(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_AVX2);
}

SDL_bool SDL_HasAVX512F(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_AVX512F);
}

SDL_bool SDL_HasAltiVec(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_ALTIVEC);
}

SDL_bool SDL_HasARMSIMD(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_ARM_SIMD);
}

SDL_bool SDL_HasARMNEON(void) {
	return CPU_FEATURE_AVAILABLE(CPU_HAS_ARM_NEON);
}


////////////////////////////////////////////////////////////////////////////////
// CPU Count
////////////////////////////////////////////////////////////////////////////////

size_t SDL_SIMDGetAlignment(void) {
	if(SDL_SIMDAlignment == 0xFFFFFFFF) {
		SDL_GetCPUFeatures();  /* make sure this has been calculated */
	}
	SDL_assert(SDL_SIMDAlignment != 0);
	return SDL_SIMDAlignment;
}

void *SDL_SIMDAlloc(const size_t len) {
	const size_t alignment = SDL_SIMDGetAlignment();
	const size_t padding = alignment - (len % alignment);
	const size_t padded = (padding != alignment) ? (len + padding) : len;
	Uint8 *retval = NULL;
	Uint8 *ptr = (Uint8 *) SDL_malloc(padded + alignment + sizeof(void *));
	if(ptr) {
		/* store the actual malloc pointer right before our aligned pointer. */
		retval = ptr + sizeof(void *);
		retval += alignment - (((size_t) retval) % alignment);
		*(((void **) retval) - 1) = ptr;
	}
	return retval;
}

void SDL_SIMDFree(void *ptr) {
	if(ptr) {
		void **realptr = (void **) ptr;
		realptr--;
		SDL_free(*(((void **) ptr) - 1));
	}
}
