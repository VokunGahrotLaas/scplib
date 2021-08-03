#ifndef SCP_COPY_H
#define SCP_COPY_H

#include "scp/utils/macros.h"

#define _SCP_MAKE_NEW_COPY_INTEGER(type, name)				\
void scpCopy_##name(const void* data, void* new_data) {		\
	*(type*)new_data = *(const type*)data;					\
}

_SCP_MAKE_NEW_COPY_INTEGER(int, i)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned, u)
_SCP_MAKE_NEW_COPY_INTEGER(long, il)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned long, ul)
#ifndef SCP_STDC_PRE99
_SCP_MAKE_NEW_COPY_INTEGER(long long, ill)
_SCP_MAKE_NEW_COPY_INTEGER(unsigned long long, ull)
#endif

#ifndef SCP_STDC_PRE11
#define scpCopy_integer(a, b) _Generic((a),		\
	int: scpCopy_i,								\
	long: scpCopy_il,							\
	long long: scpCopy_ill,						\
	unsigned: scpCopy_u,						\
	unsigned long: scpCopy_ul,					\
	unsigned long long: scpCopy_ull				\
)(a, b)
#endif

_SCP_MAKE_NEW_COPY_INTEGER(size_t, size_t)
_SCP_MAKE_NEW_COPY_INTEGER(int8_t, int8_t)
_SCP_MAKE_NEW_COPY_INTEGER(uint8_t, uint8_t)
_SCP_MAKE_NEW_COPY_INTEGER(int16_t, int16_t)
_SCP_MAKE_NEW_COPY_INTEGER(uint16_t, uint16_t)
_SCP_MAKE_NEW_COPY_INTEGER(int32_t, int32_t)
_SCP_MAKE_NEW_COPY_INTEGER(uint32_t, uint32_t)
_SCP_MAKE_NEW_COPY_INTEGER(int64_t, int64_t)
_SCP_MAKE_NEW_COPY_INTEGER(uint64_t, uint64_t)

#endif /* SCP_COPY_H */