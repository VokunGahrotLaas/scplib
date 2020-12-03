#ifndef SCP_ARRAY_H
#define SCP_ARRAY_H

#include "../utils.h"


typedef struct {
	void* data;
	size_t count;
	size_t size;
} scpArray;



scpArray* scpArray_create(size_t count, size_t size);
void scpArray_destroy(scpArray* array);
void scpArray_resize(scpArray* array, size_t count);
void* scpArray_at(scpArray* array, size_t index);
void scpArray_map_index(scpArray* array, void(*f)(size_t, size_t, void*));
void scpArray_map(scpArray* array, void(*f)(void*));
void scpArray_print(scpArray* array, void(*print_element)(void*));



scpArray* scpArray_create(size_t count, size_t size) {
	scpArray* array = malloc(sizeof(scpArray));
	array->data = calloc(count, size);
	array->count = count;
	array->size = size;
	return array;
}

void scpArray_destroy(scpArray* array) {
	free(array->data);
	free(array);
}

void scpArray_resize(scpArray* array, size_t count) {
	array->count = count;
	array->data = realloc(array->data, array->size * array->count);
}

void* scpArray_at(scpArray* array, size_t index) {
	if (index >= array->count) {
		fprintf(stderr, "scpArray_at: index can't be greater or equal to count (%i >= %i)", index, array->count);
		exit(EXIT_FAILURE);
	}
	return array->data + index * array->size;
}

void scpArray_map_index(scpArray* array, void(*f)(size_t, size_t, void*)) {
	for (size_t i = 0; i < array->count; ++i)
		f(i, array->count, array->data + i * array->size);
}

void scpArray_map(scpArray* array, void(*f)(void*)) {
	for (size_t i = 0; i < array->count; ++i)
		f(array->data + i * array->size);
}

void scpArray_print(scpArray* array, void(*print_element)(void*)) {
	void print(size_t index, size_t count, void* data) {
		print_element(data);
		if (index != count - 1)
			fputs(", ", stdout);
	}

	fputc('[', stdout);
	scpArray_map_index(array, print);
	fputc(']', stdout);
}

#endif //SCP_ARRAY_H
