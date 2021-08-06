#ifndef SCP_FPRINT_H
#define SCP_FPRINT_H

#include <stdio.h>
#include <inttypes.h>

#define _SCP_MAKE_ALL_NEW_FPRINT_INTEGER \
_SCP_MAKE_NEW_FPRINT_INTEGER(signed char, hhi, "%hhi") \
_SCP_MAKE_NEW_FPRINT_INTEGER(short, hi, "%hi") \
_SCP_MAKE_NEW_FPRINT_INTEGER(int, i, "%i") \
_SCP_MAKE_NEW_FPRINT_INTEGER(long, li, "%li") \
_SCP_MAKE_NEW_FPRINT_INTEGER(long long, lli, "%lli") \
_SCP_MAKE_NEW_FPRINT_INTEGER(unsigned char, hhu, "%hhu") \
_SCP_MAKE_NEW_FPRINT_INTEGER(unsigned short, hu, "%hu") \
_SCP_MAKE_NEW_FPRINT_INTEGER(unsigned int, u, "%u") \
_SCP_MAKE_NEW_FPRINT_INTEGER(unsigned long, lu, "%lu") \
_SCP_MAKE_NEW_FPRINT_INTEGER(unsigned long long, llu, "%llu") \
 \
_SCP_MAKE_NEW_FPRINT_INTEGER(void*, pointer, "%p") \
_SCP_MAKE_NEW_FPRINT_INTEGER(size_t, size, "%zu") \
_SCP_MAKE_NEW_FPRINT_INTEGER(intmax_t, intmax, "%ji") \
_SCP_MAKE_NEW_FPRINT_INTEGER(uintmax_t, uintmax, "%ju") \
_SCP_MAKE_NEW_FPRINT_INTEGER(ptrdiff_t, ptrdiff, "%ti") \
 \
_SCP_MAKE_NEW_FPRINT_INTEGER(int8_t, int8, "%"PRIi8) \
_SCP_MAKE_NEW_FPRINT_INTEGER(uint8_t, uint8, "%"PRIu8) \
_SCP_MAKE_NEW_FPRINT_INTEGER(int16_t, int16, "%"PRIi16) \
_SCP_MAKE_NEW_FPRINT_INTEGER(uint16_t, uint16, "%"PRIu16) \
_SCP_MAKE_NEW_FPRINT_INTEGER(int32_t, int32, "%"PRIi32) \
_SCP_MAKE_NEW_FPRINT_INTEGER(uint32_t, uint32, "%"PRIu32) \
_SCP_MAKE_NEW_FPRINT_INTEGER(int64_t, int64, "%"PRIi64) \
_SCP_MAKE_NEW_FPRINT_INTEGER(uint64_t, uint64, "%"PRIu64)

#define _SCP_MAKE_NEW_FPRINT_INTEGER(type, name, format) \
void scpFPrint_##name(FILE* stream, const void* data);

_SCP_MAKE_ALL_NEW_FPRINT_INTEGER

#undef _SCP_MAKE_NEW_FPRINT_INTEGER

#ifndef SCP_STDC_PRE11
#define scpFPrint_integer(a, b) _Generic((a), \
	signed char: scpFPrint_hhi, \
	short: scpFPrint_hi, \
	int: scpFPrint_i, \
	long: scpFPrint_li, \
	long long: scpFPrint_lli, \
	unsigned char: scpFPrint_hhu, \
	unsigned short: scpFPrint_hu, \
	unsigned int: scpFPrint_u, \
	unsigned long: scpFPrint_lu, \
	unsigned long long: scpFPrint_llu \
)(a, b)
#endif

void scpFPrint_char(FILE* stream, const void* data);
void scpFPrint_wchar(FILE* stream, const void* data);

void scpFPrint_string(FILE* stream, const void* data);
void scpFPrint_wstring(FILE* stream, const void* data);

#ifdef SCP_IMPLEMENTATION

#define _SCP_MAKE_NEW_FPRINT_INTEGER(type, name, format) \
void scpFPrint_##name(FILE* stream, const void* data) { \
	fprintf(stream, format, *(const type*)data); \
}

_SCP_MAKE_ALL_NEW_FPRINT_INTEGER

#undef _SCP_MAKE_NEW_FPRINT_INTEGER

void scpFPrint_char(FILE* stream, const void* data) { fputc((int)*(const char*)data, stream); }
void scpFPrint_wchar(FILE* stream, const void* data) { fputwc(*(const wchar_t*)data, stream); }

void scpFPrint_string(FILE* stream, const void* data) { fputs((const char*)data, stream); }
void scpFPrint_wstring(FILE* stream, const void* data) { fputws((const wchar_t*)data, stream); }

#endif // SCP_IMPLEMENTATION

#undef _SCP_MAKE_ALL_NEW_FPRINT_INTEGER

#endif // SCP_FPRINT_H
