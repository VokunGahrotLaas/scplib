#include <stdio.h>

#include "../containers/array.h"



void print_int(void* data);
void square(void* data);



int main(int argc, const char** argv) {
	scpArray* array = scpArray_create(10, sizeof(int));
	for (size_t i = 0; i < array->count; ++i)
		((int*)array->data)[i] = i;
	scpArray_print(array, print_int);
	printf("\n");

	scpArray_map(array, square);
	scpArray_print(array, print_int);
	printf("\n");

	scpArray_resize(array, 16);
	scpArray_print(array, print_int);
	printf("\n");

	scpArray_resize(array, 4);
	scpArray_print(array, print_int);
	printf("\n");

	scpArray_destroy(array);
	return EXIT_SUCCESS;
}

void print_int(void* data) {
	printf("%i", *(int*)data);
}

void square(void* data) {
	*(int*)data = *(int*)data * *(int*)data;
}