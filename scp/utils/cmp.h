#ifndef SCP_CMP_H
#define SCP_CMP_H

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define _SCP_MAKE_ALL_NEW_CMP_INTEGER \
_SCP_MAKE_NEW_CMP_INTEGER(signed char, hhi) \
_SCP_MAKE_NEW_CMP_INTEGER(short, hi) \
_SCP_MAKE_NEW_CMP_INTEGER(int, i) \
_SCP_MAKE_NEW_CMP_INTEGER(long, li) \
_SCP_MAKE_NEW_CMP_INTEGER(long long, lli) \
_SCP_MAKE_NEW_CMP_INTEGER(unsigned char, hhu) \
_SCP_MAKE_NEW_CMP_INTEGER(unsigned short, hu) \
_SCP_MAKE_NEW_CMP_INTEGER(unsigned int, u) \
_SCP_MAKE_NEW_CMP_INTEGER(unsigned long, lu) \
_SCP_MAKE_NEW_CMP_INTEGER(unsigned long long, llu)

#define _SCP_MAKE_NEW_CMP_INTEGER(type, name) \
int64_t scpCmp_##name(const void* a, const void* b);

_SCP_MAKE_ALL_NEW_CMP_INTEGER

#undef _SCP_MAKE_NEW_CMP_INTEGER

#ifndef SCP_STDC_PRE11
#define scpCmp_integer(a, b) _Generic((a), \
	signed char: scpCmp_hhi, \
	short: scpCmp_hi, \
	int: scpCmp_i, \
	long: scpCmp_li, \
	long long: scpCmp_lli, \
	unsigned char: scpCmp_hhu, \
	unsigned short: scpCmp_hu, \
	unsigned int: scpCmp_u, \
	unsigned long: scpCmp_lu, \
	unsigned long long: scpCmp_llu \
)(a, b)
#endif

int64_t scpCmp_string(const void* a, const void* b);
int64_t scpCmp_wstring(const void* a, const void* b);

#ifdef SCP_IMPLEMENTATION

#define _SCP_MAKE_NEW_CMP_INTEGER(type, name) \
int64_t scpCmp_##name(const void* a, const void* b) { \
	return (int64_t)(*(const type*)b - *(const type*)a); \
}

_SCP_MAKE_ALL_NEW_CMP_INTEGER

#undef _SCP_MAKE_NEW_CMP_INTEGER

int64_t scpCmp_string(const void* a, const void* b) {
	return strcmp((const char*)a, (const char*)b);
}

int64_t scpCmp_wstring(const void* a, const void* b) {
	return wcscmp((const wchar_t*)a, (const wchar_t*)b);
}

#endif // SCP_IMPLEMENTATION

#undef _SCP_MAKE_ALL_NEW_CMP_INTEGER

#endif // SCP_CMP_H
