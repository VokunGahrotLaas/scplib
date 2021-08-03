#ifndef SCP_BINPOW_H
#define SCP_BINPOW_H

#define _SCP_MAKE_NEW_BINPOW(short_name, long_name)									\
long_name scpMaths_binpow_ ## short_name(long_name a, long_name b) {				\
	long_name r = 1;																\
	while (b > 0) {																	\
		if (b & 1)																	\
			r *= a;																	\
		a *= a;																		\
		b >>= 1;																	\
	}																				\
	return r;																		\
}

_SCP_MAKE_NEW_BINPOW(i, int)
_SCP_MAKE_NEW_BINPOW(il, long)
_SCP_MAKE_NEW_BINPOW(ill, long long)
_SCP_MAKE_NEW_BINPOW(u, unsigned)
_SCP_MAKE_NEW_BINPOW(ul, unsigned long)
_SCP_MAKE_NEW_BINPOW(ull, unsigned long long)

#ifndef SCP_STDC99
#define scpMaths_binpow(a, b) _Generic((a),		\
	int: scpMaths_binpow_i,						\
	long: scpMaths_binpow_il,					\
	long long: scpMaths_binpow_ill,				\
	unsigned: scpMaths_binpow_u,				\
	unsigned long: scpMaths_binpow_ul,			\
	unsigned long long: scpMaths_binpow_ull		\
)(a, b)
#endif

#endif // SCP_BINPOW_H