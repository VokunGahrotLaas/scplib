#ifndef SCP_ARRAY_H
#define SCP_ARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/utils/macros.h"
#include "scp/exceptions.h"

struct scpArrayType;

struct scpArray {
	struct scpArrayType* type;
	void* data;
	size_t count;
	size_t size;
};

scpAttribute_malloc struct scpArray* scpArray_new(size_t count, size_t size);
void scpArray_delete(struct scpArray* array);
scpAttribute_malloc struct scpArray* scpArray_clone(struct scpArray* array);
scpAttribute_malloc struct scpArray* scpArray_fclone(struct scpArray* array, scpFunc_copy copy_data);
void scpArray_copy(struct scpArray* array, struct scpArray* new_array);
void scpArray_fcopy(struct scpArray* array, struct scpArray* new_array, scpFunc_copy copy_data);
void scpArray_resize(struct scpArray* array, size_t count);
void* scpArray_at(struct scpArray* array, size_t index);
void scpArray_map_index(struct scpArray* array, scpFunc_map_index f);
void scpArray_map(struct scpArray* array, scpFunc_map f);
void scpArray_print(struct scpArray* array, scpFunc_print print_element);

struct scpArrayType {
	struct scpArray* (*new)(size_t count, size_t size);
	void (*delete)(struct scpArray* array);
	struct scpArray* (*clone)(struct scpArray* array);
	struct scpArray* (*fclone)(struct scpArray* array, scpFunc_copy copy_data);
	void (*copy)(struct scpArray* array, struct scpArray* new_array);
	void (*fcopy)(struct scpArray* array, struct scpArray* new_array, scpFunc_copy copy_data);
	void (*resize)(struct scpArray* array, size_t count);
	void* (*at)(struct scpArray* array, size_t index);
	void (*map_index)(struct scpArray* array, scpFunc_map_index f);
	void (*map)(struct scpArray* array, scpFunc_map f);
	void (*print)(struct scpArray* array, scpFunc_print print_element);
} scpArray = {
	.new = scpArray_new,
	.delete = scpArray_delete,
	.clone = scpArray_clone,
	.fclone = scpArray_fclone,
	.copy = scpArray_copy,
	.fcopy = scpArray_fcopy,
	.resize = scpArray_resize,
	.at = scpArray_at,
	.map_index = scpArray_map_index,
	.map = scpArray_map,
	.print = scpArray_print,
};

struct scpArray* scpArray_new(size_t count, size_t size) {
	struct scpArray* array = (struct scpArray*)malloc(sizeof(struct scpArray));
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

struct scpArray* scpArray_clone(struct scpArray* array) {
	struct scpArray* new_array = (struct scpArray*)malloc(sizeof(struct scpArray));
	new_array->type = &scpArray;
	new_array->data = malloc(array->count * array->size);
	memcpy(new_array->data, array->data, array->count * array->size);
	new_array->count = array->count;
	new_array->size = array->size;
	return new_array;
}

struct scpArray* scpArray_fclone(struct scpArray* array, scpFunc_copy copy_data) {
	struct scpArray* new_array = (struct scpArray*)malloc(sizeof(struct scpArray));
	new_array->type = &scpArray;
	new_array->data = malloc(array->count * array->size);
	for (size_t i = 0; i < array->count * array->size; i += array->size)
		copy_data((char*)array->data + i, (char*)new_array->data + i);
	new_array->count = array->count;
	new_array->size = array->size;
	return new_array;
}

void scpArray_copy(struct scpArray* array, struct scpArray* new_array) {
	new_array->type = &scpArray;
	new_array->data = malloc(array->count * array->size);
	memcpy(new_array->data, array->data, array->count * array->size);
	new_array->count = array->count;
	new_array->size = array->size;
}

void scpArray_fcopy(struct scpArray* array, struct scpArray* new_array, scpFunc_copy copy_data) {
	new_array->type = &scpArray;
	new_array->data = malloc(array->count * array->size);
	for (size_t i = 0; i < array->count * array->size; i += array->size)
		copy_data((char*)array->data + i, (char*)new_array->data + i);
	new_array->count = array->count;
	new_array->size = array->size;
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

void scpArray_map_index(struct scpArray* array, scpFunc_map_index f) {
	for (size_t i = 0; i < array->count; ++i)
		f((void*)((char*)array->data + i * array->size), i, array->count);
}

void scpArray_map(struct scpArray* array, scpFunc_map f) {
	for (size_t i = 0; i < array->count; ++i)
		f((void*)((char*)array->data + i * array->size));
}

void scpArray_print(struct scpArray* array, scpFunc_print print_element) {
	fputc('[', stdout);
	for (size_t i = 0; i < array->count; ++i) {
		print_element((void*)((char*)array->data + i * array->size));
		if (i != array->count - 1)
			fputs(", ", stdout);
	}
	fputc(']', stdout);
}

#endif /* SCP_ARRAY_H */