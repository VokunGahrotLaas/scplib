#ifndef SCP_TESTS_UTILS_H
#define SCP_TESTS_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "scp/macros.h"

void print_uint64_t(const void* data);
void print_size_t(const void* data);
void copy_size_t(const void* data, void* new_data);
void square_size_t(void* data);

void print_uint64_t(const void* data) {
	printf("%"PRId64, *(const uint64_t*)data);
}

void print_size_t(const void* data) {
	printf("%lu", *(const size_t*)data);
}

void copy_size_t(const void* data, void* new_data) {
	*(size_t*)new_data = *(const size_t*)data;
}

void square_size_t(void* data) {
	*(size_t*)data *= *(size_t*)data;
}

#endif // SCP_TESTS_UTILS_H