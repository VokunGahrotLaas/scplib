#ifndef SCP_HASH_H
#define SCP_HASH_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

#include "scp/maths/binpow.h" 

uint64_t scpHash_base_string(const char* s, const uint64_t a);
uint64_t scpHash_stringA(const void* data);
uint64_t scpHash_stringB(const void* data);

uint64_t scpHash_base_wstring(const wchar_t* s, const uint64_t a);
uint64_t scpHash_wstringA(const void* data);
uint64_t scpHash_wstringB(const void* data);

#ifdef SCP_IMPLEMENTATION

uint64_t scpHash_base_string(const char* s, const uint64_t a) {
	uint64_t hash = 0;
	const size_t len_s = strlen(s);
	for (size_t i = 0; i < len_s; ++i)
		hash += scpMaths_binpow_uint64(a, len_s - i - 1) * (uint64_t)s[i];
	return hash;
}

uint64_t scpHash_stringA(const void* data) {
	return scpHash_base_string((const char*)data, 257);
}

uint64_t scpHash_stringB(const void* data) {
	return scpHash_base_string((const char*)data, 263);
}

uint64_t scpHash_base_wstring(const wchar_t* s, const uint64_t a) {
	uint64_t hash = 0;
	const size_t len_s = wcslen(s);
	for (size_t i = 0; i < len_s; ++i)
		hash += scpMaths_binpow_uint64(a, len_s - i - 1) * (uint64_t)s[i];
	return hash;
}

uint64_t scpHash_wstringA(const void* data) {
	return scpHash_base_wstring((const wchar_t*)data, 65537);
}

uint64_t scpHash_wstringB(const void* data) {
	return scpHash_base_wstring((const wchar_t*)data, 65539);
}

#endif // SCP_IMPLEMENTATION

#endif // SCP_HASH_H
