#ifndef SCP_PRIMES_H
#define SCP_PRIMES_H

#include "scp/containers/array.h"

struct scpArray* scpPrimes_array = NULL;

const uint64_t* scpPrimes_gen(uint64_t n);
uint64_t scpPrimes_next(uint64_t n);

scpMacro_constructor static void scpPrimes_constructor(void) {
	scpPrimes_gen(1000);
}

const uint64_t* scpPrimes_gen(uint64_t n) {
	if (!scpPrimes_array) {
		scpPrimes_array = scpArray_new(1, sizeof(uint64_t));
		*(uint64_t*)scpPrimes_array->data = 2;
	}
	for (size_t p = scpPrimes_array->count == 1 ? 3 : ((uint64_t*)scpPrimes_array->data)[scpPrimes_array->count - 1] + 2; p < n; p += 2) {
		size_t i;
		for (i = 1; i < scpPrimes_array->count \
			&& ((uint64_t*)scpPrimes_array->data)[i] * ((uint64_t*)scpPrimes_array->data)[i] < p \
			&& p % ((uint64_t*)scpPrimes_array->data)[i] != 0; ++i);
		if (i == scpPrimes_array->count || ((uint64_t*)scpPrimes_array->data)[i] * ((uint64_t*)scpPrimes_array->data)[i] >= p) {
			scpArray_resize(scpPrimes_array, scpPrimes_array->count + 1);
			((uint64_t*)scpPrimes_array->data)[scpPrimes_array->count - 1] = p;
		}
	}
	return (const uint64_t*)scpPrimes_array->data;
}

uint64_t scpPrimes_next(uint64_t n) {
	if (!scpPrimes_array) {
		scpPrimes_array = scpArray_new(1, sizeof(uint64_t));
		*(uint64_t*)scpPrimes_array->data = 2;
	}
	scpPrimes_gen(n);
	for (size_t i = 0; i < scpPrimes_array->count; ++i)
		if (((uint64_t*)scpPrimes_array->data)[i] >= n)
			return ((uint64_t*)scpPrimes_array->data)[i];
	for (uint64_t p = scpPrimes_array->count == 1 ? 3 : ((uint64_t*)scpPrimes_array->data)[scpPrimes_array->count - 1] + 2; true; p += 2) {
		size_t i;
		for (i = 1; i < scpPrimes_array->count \
			&& ((uint64_t*)scpPrimes_array->data)[i] * ((uint64_t*)scpPrimes_array->data)[i] < p \
			&& p % ((uint64_t*)scpPrimes_array->data)[i] != 0; ++i);
		if (i == scpPrimes_array->count || ((uint64_t*)scpPrimes_array->data)[i] * ((uint64_t*)scpPrimes_array->data)[i] >= p) {
			scpArray_resize(scpPrimes_array, scpPrimes_array->count + 1);
			((uint64_t*)scpPrimes_array->data)[scpPrimes_array->count - 1] = p;
			return p;
		}
	}
}

#endif // SCP_PRIMES_H