#ifndef SCP_INT128_H
#define SCP_INT128_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "scp/utils/macros.h"

#ifndef __SIZEOF_INT128__
#error "no int128 (remove -pedantic)"
#endif

#define SCP_INT64_MAX_P10 10000000000000000000ULL
#define SCP_INT64_MAX_E10 19

typedef __int128 scp_int128_t;
typedef unsigned __int128 scp_uint128_t;

int scp_fprint_uint128(FILE* stream, scp_uint128_t u);
int scp_print_uint128(scp_uint128_t u);
int scp_fprint_int128(FILE* stream, scp_int128_t i);
int scp_print_int128(scp_int128_t i);

int scp_fprint_uint128(FILE* stream, scp_uint128_t u) {
	int r;
	if (u > UINT64_MAX) {
		r = scp_fprint_uint128(stream, u / SCP_INT64_MAX_P10);
		r += printf("%." SCP_TO_STRING(SCP_INT64_MAX_E10) PRIu64, (uint64_t)(u % SCP_INT64_MAX_P10));
	} else
		r = printf("%" PRIu64, (uint64_t)u);
	return r;
}

int scp_print_uint128(scp_uint128_t u) {
	return scp_fprint_uint128(stdout, u);
}

int scp_fprint_int128(FILE* stream, scp_int128_t i) {
	uint64_t u;
	if (i < 0) {
		putc('-', stream);
		u = (uint64_t)-i;
	} else
		u = (uint64_t)i;
	return fprintf(stream, "%" PRIu64, u);
}

int scp_print_int128(scp_int128_t i) {
	return scp_fprint_int128(stdout, i);
}

#endif // SCP_INT128_H