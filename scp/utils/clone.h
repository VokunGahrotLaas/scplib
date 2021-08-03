#ifndef SCP_CLONE_H
#define SCP_CLONE_H

#include "scp/utils/macros.h"

#define _SCP_MAKE_NEW_CLONE_INTEGER(type, name)					\
scpAttribute_malloc void* scpClone_##name(const void* data) {	\
	void* new_data = malloc(sizeof(type));						\
	*(type*)new_data = *(type*)data;							\
	return new_data;											\
}

_SCP_MAKE_NEW_CLONE_INTEGER(int, i)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned, u)
_SCP_MAKE_NEW_CLONE_INTEGER(long, il)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned long, ul)
#ifndef SCP_STDC_PRE99
_SCP_MAKE_NEW_CLONE_INTEGER(long long, ill)
_SCP_MAKE_NEW_CLONE_INTEGER(unsigned long long, ull)
#endif

#ifndef SCP_STDC_PRE11
#define scpClone_integer(a, b) _Generic((a),	\
	int: scpClone_i,							\
	long: scpClone_il,							\
	long long: scpClone_ill,					\
	unsigned: scpClone_u,						\
	unsigned long: scpClone_ul,					\
	unsigned long long: scpClone_ull			\
)(a, b)
#endif

_SCP_MAKE_NEW_CLONE_INTEGER(size_t, size_t)
_SCP_MAKE_NEW_CLONE_INTEGER(int8_t, int8_t)
_SCP_MAKE_NEW_CLONE_INTEGER(uint8_t, uint8_t)
_SCP_MAKE_NEW_CLONE_INTEGER(int16_t, int16_t)
_SCP_MAKE_NEW_CLONE_INTEGER(uint16_t, uint16_t)
_SCP_MAKE_NEW_CLONE_INTEGER(int32_t, int32_t)
_SCP_MAKE_NEW_CLONE_INTEGER(uint32_t, uint32_t)
_SCP_MAKE_NEW_CLONE_INTEGER(int64_t, int64_t)
_SCP_MAKE_NEW_CLONE_INTEGER(uint64_t, uint64_t)

#endif /* SCP_CLONE_H */