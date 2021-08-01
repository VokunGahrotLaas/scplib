#ifndef SCP_TESTS_UTILS_H
#define SCP_TESTS_UTILS_H

#include <stdlib.h>
#include <stdio.h>

void print_size_t(void* data);
void copy_size_t(void* data, void* new_data);
void square_size_t(void* data);

void print_size_t(void* data) {
	printf("%lu", *(size_t*)data);
}

void copy_size_t(void* data, void* new_data) {
	*(size_t*)new_data = *(size_t*)data;
}

void square_size_t(void* data) {
	*(size_t*)data *= *(size_t*)data;
}

#endif // SCP_TESTS_UTILS_H