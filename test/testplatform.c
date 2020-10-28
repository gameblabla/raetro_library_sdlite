#include <stdio.h>

#include "SDL.h"
#include "SDL_endian.h"
#include "SDL_cpuinfo.h"

static int badsize(size_t sizeoftype, size_t hardcodetype) {
	return sizeoftype != hardcodetype;
}

int TestTypes(SDL_bool verbose) {
	int error = 0;

	SDL_COMPILE_TIME_ASSERT(SDL_MAX_SINT8, SDL_MAX_SINT8 == 127);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_SINT8, SDL_MIN_SINT8 == -128);
	SDL_COMPILE_TIME_ASSERT(SDL_MAX_UINT8, SDL_MAX_UINT8 == 255);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_UINT8, SDL_MIN_UINT8 == 0);

	SDL_COMPILE_TIME_ASSERT(SDL_MAX_SINT16, SDL_MAX_SINT16 == 32767);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_SINT16, SDL_MIN_SINT16 == -32768);
	SDL_COMPILE_TIME_ASSERT(SDL_MAX_UINT16, SDL_MAX_UINT16 == 65535);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_UINT16, SDL_MIN_UINT16 == 0);

	SDL_COMPILE_TIME_ASSERT(SDL_MAX_SINT32, SDL_MAX_SINT32 == 2147483647);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_SINT32, SDL_MIN_SINT32 == ~0x7fffffff); /* Instead of -2147483648, which is treated as unsigned by some compilers */
	SDL_COMPILE_TIME_ASSERT(SDL_MAX_UINT32, SDL_MAX_UINT32 == 4294967295u);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_UINT32, SDL_MIN_UINT32 == 0);

	SDL_COMPILE_TIME_ASSERT(SDL_MAX_SINT64, SDL_MAX_SINT64 == 9223372036854775807ll);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_SINT64, SDL_MIN_SINT64 == ~0x7fffffffffffffffll); /* Instead of -9223372036854775808, which is treated as unsigned by compilers */
	SDL_COMPILE_TIME_ASSERT(SDL_MAX_UINT64, SDL_MAX_UINT64 == 18446744073709551615ull);
	SDL_COMPILE_TIME_ASSERT(SDL_MIN_UINT64, SDL_MIN_UINT64 == 0);

	if(badsize(sizeof(Uint8), 1)) {
		if(verbose) {
			SDL_Log("sizeof(Uint8) != 1, instead = %u\n", (unsigned int) sizeof(Uint8));
		}
		++error;
	}
	if(badsize(sizeof(Uint16), 2)) {
		if(verbose) {
			SDL_Log("sizeof(Uint16) != 2, instead = %u\n", (unsigned int) sizeof(Uint16));
		}
		++error;
	}
	if(badsize(sizeof(Uint32), 4)) {
		if(verbose) {
			SDL_Log("sizeof(Uint32) != 4, instead = %u\n", (unsigned int) sizeof(Uint32));
		}
		++error;
	}
	if(badsize(sizeof(Uint64), 8)) {
		if(verbose) {
			SDL_Log("sizeof(Uint64) != 8, instead = %u\n", (unsigned int) sizeof(Uint64));
		}
		++error;
	}
	if(verbose && !error) {
		SDL_Log("All data types are the expected size.\n");
	}

	return (error ? 1 : 0);
}

int TestEndian(SDL_bool verbose) {
	int error = 0;
	Uint16 value = 0x1234;
	int real_byteorder;
	Uint16 value16 = 0xCDAB;
	Uint16 swapped16 = 0xABCD;
	Uint32 value32 = 0xEFBEADDE;
	Uint32 swapped32 = 0xDEADBEEF;
#ifdef SDL_HAS_64BIT_TYPE
	Uint64 value64, swapped64;
	value64 = 0xEFBEADDE;
	value64 <<= 32;
	value64 |= 0xCDAB3412;
	swapped64 = 0x1234ABCD;
	swapped64 <<= 32;
	swapped64 |= 0xDEADBEEF;
#endif

	if(verbose) {
		printf("Detected a %s endian machine.\n", (SDL_BYTEORDER == SDL_LIL_ENDIAN) ? "little" : "big");
	}
	if((*((char *) &value) >> 4) == 0x1) {
		real_byteorder = SDL_BIG_ENDIAN;
	} else {
		real_byteorder = SDL_LIL_ENDIAN;
	}
	if(real_byteorder != SDL_BYTEORDER) {
		if(verbose) {
			printf("Actually a %s endian machine!\n", (real_byteorder == SDL_LIL_ENDIAN) ? "little" : "big");
		}
		++error;
	}
	if(verbose) {
		printf("Value 16 = 0x%X, swapped = 0x%X\n", value16, SDL_Swap16(value16));
	}
	if(SDL_Swap16(value16) != swapped16) {
		if(verbose) {
			printf("16 bit value swapped incorrectly!\n");
		}
		++error;
	}
	if(verbose) {
		printf("Value 32 = 0x%X, swapped = 0x%X\n", value32, SDL_Swap32(value32));
	}
	if(SDL_Swap32(value32) != swapped32) {
		if(verbose) {
			printf("32 bit value swapped incorrectly!\n");
		}
		++error;
	}
#ifdef SDL_HAS_64BIT_TYPE
	if(verbose) {
		printf("Value 64 = 0x%llX, swapped = 0x%llX\n", (unsigned long long) value64, (unsigned long long) SDL_Swap64(value64));
	}
	if(SDL_Swap64(value64) != swapped64) {
		if(verbose) {
			printf("64 bit value swapped incorrectly!\n");
		}
		++error;
	}
#endif
	return (error ? 1 : 0);
}

int TestCPUInfo(SDL_bool verbose) {
	if(verbose) {
		printf("CPU count: %d\n", SDL_GetCPUCount());
		printf("CPU cache line size: %d\n", SDL_GetCPUCacheLineSize());
		printf("RDTSC %s\n", SDL_HasRDTSC() ? "detected" : "not detected");
		printf("MMX %s\n", SDL_HasMMX() ? "detected" : "not detected");
		printf("MMX Ext %s\n", SDL_HasMMXExt() ? "detected" : "not detected");
		printf("3DNow %s\n", SDL_Has3DNow() ? "detected" : "not detected");
		printf("3DNow Ext %s\n", SDL_Has3DNowExt() ? "detected" : "not detected");
		printf("SSE %s\n", SDL_HasSSE() ? "detected" : "not detected");
		printf("SSE2 %s\n", SDL_HasSSE2() ? "detected" : "not detected");
		printf("SSE3 %s\n", SDL_HasSSE3() ? "detected" : "not detected");
		printf("SSE4.1 %s\n", SDL_HasSSE41() ? "detected" : "not detected");
		printf("SSE4.2 %s\n", SDL_HasSSE42() ? "detected" : "not detected");
		printf("AVX %s\n", SDL_HasAVX() ? "detected" : "not detected");
		printf("AVX2 %s\n", SDL_HasAVX2() ? "detected" : "not detected");
		printf("AVX-512F %s\n", SDL_HasAVX512F() ? "detected" : "not detected");
		printf("ARMv6 SIMD %s\n", SDL_HasARMSIMD() ? "detected" : "not detected");
		printf("NEON %s\n", SDL_HasARMNEON() ? "detected" : "not detected");
		printf("System RAM %d MB\n", SDL_GetSystemRAM());

	}
	return (0);
}

int main(int argc, char *argv[]) {
	SDL_bool verbose = SDL_TRUE;
	int status = 0;

	if(argv[1] && (SDL_strcmp(argv[1], "-q") == 0)) {
		verbose = SDL_FALSE;
	}
	if(verbose) {
		printf("This system is running %s\n", SDL_GetPlatform());
	}

	status += TestTypes(verbose);
	status += TestEndian(verbose);
	status += TestCPUInfo(verbose);
	return status;
}
