#ifndef SCP_TESTS_UTILS_H
#define SCP_TESTS_UTILS_H

#include <stdlib.h>
#include <stdio.h>

void print_size_t(void* data);
void square(void* data);

void print_size_t(void* data) {
	printf("%lu", *(size_t*)data);
}

void square(void* data) {
	*(size_t*)data *= *(size_t*)data;
}

#endif // SCP_TESTS_UTILS_H