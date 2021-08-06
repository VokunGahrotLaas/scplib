#ifndef SCP_PRIMES_H
#define SCP_PRIMES_H

#include <stdint.h>

const uint64_t* scpPrimes_gen(uint64_t n);
uint64_t scpPrimes_next(uint64_t n);

#ifdef SCP_IMPLEMENTATION

#include "scp/containers/vector.h"

static struct scpVector* scpPrimes_vector = NULL;

const uint64_t* scpPrimes_gen(uint64_t n) {
	if (!scpPrimes_vector) {
		scpPrimes_vector = scpVector_new(100, sizeof(uint64_t));
		*(uint64_t*)scpVector_append(scpPrimes_vector) = 2;
	}
	for (size_t p = scpPrimes_vector->count == 1 ? 3 : ((uint64_t*)scpPrimes_vector->data)[scpPrimes_vector->count - 1] + 2; p < n; p += 2) {
		size_t i;
		for (i = 1; i < scpPrimes_vector->count \
			&& ((uint64_t*)scpPrimes_vector->data)[i] * ((uint64_t*)scpPrimes_vector->data)[i] < p \
			&& p % ((uint64_t*)scpPrimes_vector->data)[i] != 0; ++i);
		if (i == scpPrimes_vector->count || ((uint64_t*)scpPrimes_vector->data)[i] * ((uint64_t*)scpPrimes_vector->data)[i] >= p)
			*(uint64_t*)scpVector_append(scpPrimes_vector) = p;
	}
	return (const uint64_t*)scpPrimes_vector->data;
}

uint64_t scpPrimes_next(uint64_t n) {
	scpPrimes_gen(n);
	for (size_t i = 0; i < scpPrimes_vector->count; ++i)
		if (((uint64_t*)scpPrimes_vector->data)[i] >= n)
			return ((uint64_t*)scpPrimes_vector->data)[i];
	for (uint64_t p = scpPrimes_vector->count == 1 ? 3 : ((uint64_t*)scpPrimes_vector->data)[scpPrimes_vector->count - 1] + 2; true; p += 2) {
		size_t i;
		for (i = 1; i < scpPrimes_vector->count \
			&& ((uint64_t*)scpPrimes_vector->data)[i] * ((uint64_t*)scpPrimes_vector->data)[i] < p \
			&& p % ((uint64_t*)scpPrimes_vector->data)[i] != 0; ++i);
		if (i == scpPrimes_vector->count || ((uint64_t*)scpPrimes_vector->data)[i] * ((uint64_t*)scpPrimes_vector->data)[i] >= p) {
			*(uint64_t*)scpVector_append(scpPrimes_vector) = p;
			return p;
		}
	}
}

#endif // SCP_IMPLEMENTATION

#endif // SCP_PRIMES_H
