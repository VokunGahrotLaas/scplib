#include <stdio.h>

#include "scp/containers/array.h"

static inline void print_size_t(void* data);
static inline void square(void* data);

int main(void) {
	scpArray* array = scpArray_create(10, sizeof(size_t));
	for (size_t i = 0; i < array->count; ++i)
		((size_t*)array->data)[i] = i;
	scpArray_print(array, print_size_t);
	printf("\n");

	scpArray_map(array, square);
	scpArray_print(array, print_size_t);
	printf("\n");

	scpArray_resize(array, 16);
	scpArray_print(array, print_size_t);
	printf("\n");

	scpArray_resize(array, 4);
	scpArray_print(array, print_size_t);
	printf("\n");

	scpArray_at(array, (size_t)-1);

	scpArray_destroy(array);
	return EXIT_SUCCESS;
}

static inline void print_size_t(void* data) {
	printf("%lu", *(size_t*)data);
}

static inline void square(void* data) {
	*(size_t*)data *= *(size_t*)data;
}