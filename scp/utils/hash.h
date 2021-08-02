#ifndef SCP_HASH_H
#define SCP_HASH_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "scp/maths/binpow.h"

uint64_t scpHash_base_string(const char* s, const uint64_t a) {
	uint64_t hash = 0;
	const size_t len_s = strlen(s);
	for (size_t i = 0; i < len_s; ++i)
		hash += scpMaths_binpow(a, len_s - i - 1) * (uint64_t)s[i];
	return hash;
}

uint64_t scpHash_stringA(const void* data) {
	return scpHash_base_string((const char*)data, 257);
}

uint64_t scpHash_stringB(const void* data) {
	return scpHash_base_string((const char*)data, 263);
}

#endif // SCP_HASH_H