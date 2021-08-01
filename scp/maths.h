#ifndef SCP_MATHS_H
#define SCP_MATHS_H

#define _SCP_MAKE_NEW_POW(short_name, long_name)										\
long_name scpMaths_pow_ ## short_name(long_name a, long_name b) {					\
	long_name r = 1;																\
	while (b > 0) {																	\
		if (b & 1)																	\
			r *= a;																	\
		a *= a;																		\
		b >>= 1;																	\
	}																				\
	return r;																		\
}

_SCP_MAKE_NEW_POW(i, int)
_SCP_MAKE_NEW_POW(il, long)
_SCP_MAKE_NEW_POW(ill, long long)
_SCP_MAKE_NEW_POW(u, unsigned)
_SCP_MAKE_NEW_POW(ul, unsigned long)
_SCP_MAKE_NEW_POW(ull, unsigned long long)

#define scpMaths_pow(a, b) _Generic((a),	\
	int: scpMaths_pow_i,					\
	long: scpMaths_pow_il,					\
	long long: scpMaths_pow_ill,			\
	unsigned: scpMaths_pow_u,				\
	unsigned long: scpMaths_pow_ul,			\
	unsigned long long: scpMaths_pow_ull	\
)(a, b)

#endif // SCP_MATHS_H