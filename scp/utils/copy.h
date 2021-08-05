#ifndef SCP_COPY_H
#define SCP_COPY_H

#include "scp/utils/macros.h"

#define _SCP_MAKE_NEW_COPY_INTEGER(type, name)				\
void scpCopy_##name(const void* data, void* new_data) {		\
	*(type*)new_data = (type)*(const type*)data;			\
}

_SCP_MAKE_NEW_COPY_INTEGER(signed char, hhi)
_SCP_MAKE_NEW_COPY_INTEGER(short, hi)
_SCP_MAKE_NEW_COPY_INTEGER(int, i)
_SCP_MAKE_NEW_COPY_INTEGER(long, li)
_SCP_MAKE_NEW_COPY_INTEGER(long long, lli)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned char, hhu)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned short, hu)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned int, u)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned long, lu)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned long long, llu)

_SCP_MAKE_NEW_COPY_INTEGER(void*, pointer)
_SCP_MAKE_NEW_COPY_INTEGER(size_t, size)
_SCP_MAKE_NEW_COPY_INTEGER(intmax_t, intmax)
_SCP_MAKE_NEW_COPY_INTEGER(uintmax_t, uintmax)
_SCP_MAKE_NEW_COPY_INTEGER(ptrdiff_t, ptrdiff)

_SCP_MAKE_NEW_COPY_INTEGER(int8_t, int8)
_SCP_MAKE_NEW_COPY_INTEGER(uint8_t, uint8)
_SCP_MAKE_NEW_COPY_INTEGER(int16_t, int16)
_SCP_MAKE_NEW_COPY_INTEGER(uint16_t, uint16)
_SCP_MAKE_NEW_COPY_INTEGER(int32_t, int32)
_SCP_MAKE_NEW_COPY_INTEGER(uint32_t, uint32)
_SCP_MAKE_NEW_COPY_INTEGER(int64_t, int64)
_SCP_MAKE_NEW_COPY_INTEGER(uint64_t, uint64)

#undef _SCP_MAKE_NEW_COPY_INTEGER

#ifndef SCP_STDC_PRE11
#define scpCopy_integer(a, b) _Generic((a),		\
	signed char: scpCopy_hhi,					\
	short: scpCopy_hi,							\
	int: scpCopy_i,								\
	long: scpCopy_li,							\
	long long: scpCopy_lli,						\
	unsigned char: scpCopy_hhu,					\
	unsigned short: scpCopy_hu,					\
	unsigned int: scpCopy_u,					\
	unsigned long: scpCopy_lu,					\
	unsigned long long: scpCopy_llu				\
)(a, b)
#endif

#endif // SCP_COPY_H
