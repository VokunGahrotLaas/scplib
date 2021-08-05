#ifndef SCP_PRINT_H
#define SCP_PRINT_H

#include <stdio.h>
#include <inttypes.h>

#define _SCP_MAKE_NEW_PRINT_INTEGER(type, name, format)		\
void scpPrint_##name(const void* data) {					\
	printf(format, *(const type*)data);						\
}

_SCP_MAKE_NEW_PRINT_INTEGER(signed char, hhi, "%hhi")
_SCP_MAKE_NEW_PRINT_INTEGER(short, hi, "%hi")
_SCP_MAKE_NEW_PRINT_INTEGER(int, i, "%i")
_SCP_MAKE_NEW_PRINT_INTEGER(long, li, "%li")
_SCP_MAKE_NEW_PRINT_INTEGER(long long, lli, "%lli")
_SCP_MAKE_NEW_PRINT_INTEGER(unsigned char, hhu, "%hhu")
_SCP_MAKE_NEW_PRINT_INTEGER(unsigned short, hu, "%hu")
_SCP_MAKE_NEW_PRINT_INTEGER(unsigned int, u, "%u")
_SCP_MAKE_NEW_PRINT_INTEGER(unsigned long, lu, "%lu")
_SCP_MAKE_NEW_PRINT_INTEGER(unsigned long long, llu, "%llu")

_SCP_MAKE_NEW_PRINT_INTEGER(void*, pointer, "%p")
_SCP_MAKE_NEW_PRINT_INTEGER(size_t, size, "%zu")
_SCP_MAKE_NEW_PRINT_INTEGER(intmax_t, intmax, "%ji")
_SCP_MAKE_NEW_PRINT_INTEGER(uintmax_t, uintmax, "%ju")
_SCP_MAKE_NEW_PRINT_INTEGER(ptrdiff_t, ptrdiff, "%ti")

_SCP_MAKE_NEW_PRINT_INTEGER(int8_t, int8, "%"PRIi8)
_SCP_MAKE_NEW_PRINT_INTEGER(uint8_t, uint8, "%"PRIu8)
_SCP_MAKE_NEW_PRINT_INTEGER(int16_t, int16, "%"PRIi16)
_SCP_MAKE_NEW_PRINT_INTEGER(uint16_t, uint16, "%"PRIu16)
_SCP_MAKE_NEW_PRINT_INTEGER(int32_t, int32, "%"PRIi32)
_SCP_MAKE_NEW_PRINT_INTEGER(uint32_t, uint32, "%"PRIu32)
_SCP_MAKE_NEW_PRINT_INTEGER(int64_t, int64, "%"PRIi64)
_SCP_MAKE_NEW_PRINT_INTEGER(uint64_t, uint64, "%"PRIu64)

#undef _SCP_MAKE_NEW_PRINT_INTEGER

#ifndef SCP_STDC_PRE11
#define scpPrint_integer(a, b) _Generic((a),	\
	signed char: scpPrint_hhi,					\
	short: scpPrint_hi,							\
	int: scpPrint_i,							\
	long: scpPrint_li,							\
	long long: scpPrint_lli,					\
	unsigned char: scpPrint_hhu,				\
	unsigned short: scpPrint_hu,				\
	unsigned int: scpPrint_u,					\
	unsigned long: scpPrint_lu,					\
	unsigned long long: scpPrint_llu			\
)(a, b)
#endif

void scpPrint_char(const void* data) { fputc((int)*(const char*)data, stdout); }
void scpPrint_wchar(const void* data) { fputwc(*(const wchar_t*)data, stdout); }

void scpPrint_string(const void* data) { fputs((const char*)data, stdout); }
void scpPrint_wstring(const void* data) { fputws((const wchar_t*)data, stdout); }

#endif // SCP_PRINT_H
