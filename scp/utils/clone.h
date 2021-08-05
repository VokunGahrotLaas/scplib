#ifndef SCP_CLONE_H
#define SCP_CLONE_H

#include "scp/utils/macros.h"

#define _SCP_MAKE_NEW_CLONE_INTEGER(type, name)					\
scpAttribute_malloc void* scpClone_##name(const void* data) {	\
	void* new_data = malloc(sizeof(type));						\
	*(type*)new_data = (type)*(const type*)data;				\
	return new_data;											\
}

_SCP_MAKE_NEW_CLONE_INTEGER(signed char, hhi)
_SCP_MAKE_NEW_CLONE_INTEGER(short, hi)
_SCP_MAKE_NEW_CLONE_INTEGER(int, i)
_SCP_MAKE_NEW_CLONE_INTEGER(long, li)
_SCP_MAKE_NEW_CLONE_INTEGER(long long, lli)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned char, hhu)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned short, hu)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned int, u)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned long, lu)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned long long, llu)

_SCP_MAKE_NEW_CLONE_INTEGER(void*, pointer)
_SCP_MAKE_NEW_CLONE_INTEGER(size_t, size)
_SCP_MAKE_NEW_CLONE_INTEGER(intmax_t, intmax)
_SCP_MAKE_NEW_CLONE_INTEGER(uintmax_t, uintmax)
_SCP_MAKE_NEW_CLONE_INTEGER(ptrdiff_t, ptrdiff)

_SCP_MAKE_NEW_CLONE_INTEGER(int8_t, int8)
_SCP_MAKE_NEW_CLONE_INTEGER(uint8_t, uint8)
_SCP_MAKE_NEW_CLONE_INTEGER(int16_t, int16)
_SCP_MAKE_NEW_CLONE_INTEGER(uint16_t, uint16)
_SCP_MAKE_NEW_CLONE_INTEGER(int32_t, int32)
_SCP_MAKE_NEW_CLONE_INTEGER(uint32_t, uint32)
_SCP_MAKE_NEW_CLONE_INTEGER(int64_t, int64)
_SCP_MAKE_NEW_CLONE_INTEGER(uint64_t, uint64)

#undef _SCP_MAKE_NEW_CLONE_INTEGER

#ifndef SCP_STDC_PRE11
#define scpClone_integer(a, b) _Generic((a),	\
	signed char: scpClone_hhi,					\
	short: scpClone_hi,							\
	int: scpClone_i,							\
	long: scpClone_li,							\
	long long: scpClone_lli,					\
	unsigned char: scpClone_hhu,				\
	unsigned short: scpClone_hu,				\
	unsigned int: scpClone_u,					\
	unsigned long: scpClone_lu,					\
	unsigned long long: scpClone_llu			\
)(a, b)
#endif

#endif /* SCP_CLONE_H */