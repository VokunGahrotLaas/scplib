#ifndef SCP_ARRAY_H
#define SCP_ARRAY_H

#include <scp/exceptions.h>

typedef struct scpArray {
	void* data;
	size_t count;
	size_t size;
} scpArray;

scpArray* scpArray_create(size_t count, size_t size);
void scpArray_destroy(scpArray* array);
void scpArray_resize(scpArray* array, size_t count);
void* scpArray_at(scpArray* array, size_t index);
void scpArray_map_index(scpArray* array, void(*f)(void*, size_t, size_t));
void scpArray_map(scpArray* array, void(*f)(void*));
void scpArray_print(scpArray* array, void(*print_element)(void*));

scpArray* scpArray_create(size_t count, size_t size) {
	scpArray* array = (scpArray*)malloc(sizeof(scpArray));
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
	if (index >= array->count)
		scpException_OutOfBoundf("scpArray_at: index can't be greater or equal to count (%lu >= %lu)", index, array->count);
	return (void*)((char*)array->data + index * array->size);
}

void scpArray_map_index(scpArray* array, void(*f)(void*, size_t, size_t)) {
	for (size_t i = 0; i < array->count; ++i)
		f((void*)((char*)array->data + i * array->size), i, array->count);
}

void scpArray_map(scpArray* array, void(*f)(void*)) {
	for (size_t i = 0; i < array->count; ++i)
		f((void*)((char*)array->data + i * array->size));
}

void scpArray_print(scpArray* array, void(*print_element)(void*)) {
	fputc('[', stdout);
	for (size_t i = 0; i < array->count; ++i) {
		print_element((void*)((char*)array->data + i * array->size));
		if (i != array->count - 1)
			fputs(", ", stdout);
	}
	fputc(']', stdout);
}

#endif // SCP_ARRAY_H