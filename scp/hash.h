#ifndef SCP_HASH_H
#define SCP_HASH_H

#include <stdlib.h>
#include <stdint.h>

#include "scp/maths.h"

uint64_t scpHash_string(void* data) {
	const char* s = (const char*)data;
	const uint64_t a = 151;
	const uint64_t m = 53;
	uint64_t hash = 0;
	const size_t len_s = strlen(s);
	for (size_t i = 0; i < len_s; ++i) {
		hash += scpMaths_pow(a, len_s - i - 1) * (uint64_t)s[i];
		hash %= m;
	}
	return hash;
}

#endif // SCP_HASH_H