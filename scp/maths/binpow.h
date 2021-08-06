#ifndef SCP_BINPOW_H
#define SCP_BINPOW_H

#include "scp/utils/macros.h"

#define _SCP_MAKE_ALL_NEW_BINPOW \
_SCP_MAKE_NEW_BINPOW(signed char, hhi) \
_SCP_MAKE_NEW_BINPOW(short, hi) \
_SCP_MAKE_NEW_BINPOW(int, i) \
_SCP_MAKE_NEW_BINPOW(long, li) \
_SCP_MAKE_NEW_BINPOW(long long, lli) \
_SCP_MAKE_NEW_BINPOW(unsigned char, hhu) \
_SCP_MAKE_NEW_BINPOW(unsigned short, hu) \
_SCP_MAKE_NEW_BINPOW(unsigned int, u) \
_SCP_MAKE_NEW_BINPOW(unsigned long, lu) \
_SCP_MAKE_NEW_BINPOW(unsigned long long, llu) \
 \
_SCP_MAKE_NEW_BINPOW(size_t, size) \
_SCP_MAKE_NEW_BINPOW(intmax_t, intmax) \
_SCP_MAKE_NEW_BINPOW(uintmax_t, uintmax) \
_SCP_MAKE_NEW_BINPOW(ptrdiff_t, ptrdiff) \
 \
_SCP_MAKE_NEW_BINPOW(int8_t, int8) \
_SCP_MAKE_NEW_BINPOW(uint8_t, uint8) \
_SCP_MAKE_NEW_BINPOW(int16_t, int16) \
_SCP_MAKE_NEW_BINPOW(uint16_t, uint16) \
_SCP_MAKE_NEW_BINPOW(int32_t, int32) \
_SCP_MAKE_NEW_BINPOW(uint32_t, uint32) \
_SCP_MAKE_NEW_BINPOW(int64_t, int64) \
_SCP_MAKE_NEW_BINPOW(uint64_t, uint64)

#define _SCP_MAKE_NEW_BINPOW(long_name, short_name) \
long_name scpMaths_binpow_##short_name(long_name a, long_name b);

_SCP_MAKE_ALL_NEW_BINPOW

#undef _SCP_MAKE_NEW_BINPOW

#ifndef SCP_STDC_PRE11
#define scpMaths_binpow(a, b) _Generic((a), \
	signed char: scpMaths_binpow_hhi, \
	short: scpMaths_binpow_hi, \
	int: scpMaths_binpow_i, \
	long: scpMaths_binpow_li, \
	long long: scpMaths_binpow_lli, \
	unsigned char: scpMaths_binpow_hhu, \
	unsigned short: scpMaths_binpow_hu, \
	unsigned int: scpMaths_binpow_u, \
	unsigned long: scpMaths_binpow_lu, \
	unsigned long long: scpMaths_binpow_llu \
)(a, b)
#endif

#ifdef SCP_IMPLEMENTATION

#define _SCP_MAKE_NEW_BINPOW(long_name, short_name) \
long_name scpMaths_binpow_##short_name(long_name a, long_name b) { \
	long_name r = 1; \
	while (b > 0) { \
		if (b & 1) \
			r *= a; \
		a *= a; \
		b >>= 1; \
	} \
	return r; \
}

_SCP_MAKE_ALL_NEW_BINPOW

#endif // SCP_IMPLEMENTATION

#undef _SCP_MAKE_ALL_NEW_BINPOW

#endif // SCP_BINPOW_H
