#ifndef SCP_ARRAY_H
#define SCP_ARRAY_H

#include <scp/exceptions.h>

struct scpArrayType;

struct scpArray {
	struct scpArrayType* type;
	void* data;
	size_t count;
	size_t size;
};

struct scpArray* scpArray_new(size_t count, size_t size);
void scpArray_delete(struct scpArray* array);
void scpArray_resize(struct scpArray* array, size_t count);
void* scpArray_at(struct scpArray* array, size_t index);
void scpArray_map_index(struct scpArray* array, void(*f)(void* data, size_t i, size_t count));
void scpArray_map(struct scpArray* array, void(*f)(void* data));
void scpArray_print(struct scpArray* array, void(*print_element)(void* data));

struct scpArrayType {
	struct scpArray* (*new)(size_t count, size_t size);
	void (*delete)(struct scpArray* array);
	void (*resize)(struct scpArray* array, size_t count);
	void* (*at)(struct scpArray* array, size_t index);
	void (*map_index)(struct scpArray* array, void(*f)(void* data, size_t index, size_t count));
	void (*map)(struct scpArray* array, void(*f)(void* data));
	void (*print)(struct scpArray* array, void(*print_element)(void* data));
} scpArray = {
	.new = scpArray_new,
	.delete = scpArray_delete,
	.resize = scpArray_resize,
	.at = scpArray_at,
	.map_index = scpArray_map_index,
	.map = scpArray_map,
	.print = scpArray_print,
};

struct scpArray* scpArray_new(size_t count, size_t size) {
	struct scpArray* array = (struct scpArray*)malloc(sizeof(scpArray));
	array->type = &scpArray;
	array->data = calloc(count, size);
	array->count = count;
	array->size = size;
	return array;
}

void scpArray_delete(struct scpArray* array) {
	free(array->data);
	free(array);
}

void scpArray_resize(struct scpArray* array, size_t count) {
	array->count = count;
	array->data = realloc(array->data, array->size * array->count);
}

void* scpArray_at(struct scpArray* array, size_t index) {
	if (index >= array->count)
		SCP_EXCEPTION(scpException_Exception, "index can't be greater or equal to count (%lu >= %lu)", index, array->count);
	return (void*)((char*)array->data + index * array->size);
}

void scpArray_map_index(struct scpArray* array, void(*f)(void* data, size_t i, size_t count)) {
	for (size_t i = 0; i < array->count; ++i)
		f((void*)((char*)array->data + i * array->size), i, array->count);
}

void scpArray_map(struct scpArray* array, void(*f)(void* data)) {
	for (size_t i = 0; i < array->count; ++i)
		f((void*)((char*)array->data + i * array->size));
}

void scpArray_print(struct scpArray* array, void(*print_element)(void* data)) {
	fputc('[', stdout);
	for (size_t i = 0; i < array->count; ++i) {
		print_element((void*)((char*)array->data + i * array->size));
		if (i != array->count - 1)
			fputs(", ", stdout);
	}
	fputc(']', stdout);
}

#endif // SCP_ARRAY_H