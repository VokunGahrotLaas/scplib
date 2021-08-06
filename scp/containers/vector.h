#ifndef SCP_VECTOR_H
#define SCP_VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/utils/macros.h"
#include "scp/exceptions.h"

struct scpVectorType;

struct scpVector {
	const struct scpVectorType* type;
	void* data;
	size_t count;
	size_t size;
	size_t reserved;
};

scpAttribute_malloc
struct scpVector* scpVector_new(size_t count, size_t size);
void scpVector_delete(struct scpVector* vector);
scpAttribute_malloc
struct scpVector* scpVector_clone(struct scpVector* vector);
scpAttribute_malloc
struct scpVector* scpVector_fclone(struct scpVector* vector, scpFunc_copy copy_data);
void scpVector_copy(struct scpVector* vector, struct scpVector* new_vector);
void scpVector_fcopy(struct scpVector* vector, struct scpVector* new_vector, scpFunc_copy copy_data);
void scpVector_resize(struct scpVector* vector, size_t count);
void scpVector_reserve(struct scpVector* vector, size_t count);
void* scpVector_at(struct scpVector* vector, size_t index);
void* scpVector_append(struct scpVector* vector);
void scpVector_pop(struct scpVector* vector);
void scpVector_map_index(struct scpVector* vector, scpFunc_map_index f);
void scpVector_map(struct scpVector* vector, scpFunc_map f);
void scpVector_print(struct scpVector* vector, scpFunc_print print);
void scpVector_fprint(struct scpVector* vector, FILE* stream, scpFunc_fprint fprint);

static struct scpVectorType {
	struct scpVector* (*new)(size_t count, size_t size);
	void (*delete)(struct scpVector* vector);
	struct scpVector* (*clone)(struct scpVector* vector);
	struct scpVector* (*fclone)(struct scpVector* vector, scpFunc_copy copy_data);
	void (*copy)(struct scpVector* vector, struct scpVector* new_vector);
	void (*fcopy)(struct scpVector* vector, struct scpVector* new_vector, scpFunc_copy copy_data);
	void (*resize)(struct scpVector* vector, size_t count);
	void (*reserve)(struct scpVector* vector, size_t count);
	void* (*at)(struct scpVector* vector, size_t index);
	void* (*append)(struct scpVector* vector);
	void (*pop)(struct scpVector* vector);
	void (*map_index)(struct scpVector* vector, scpFunc_map_index f);
	void (*map)(struct scpVector* vector, scpFunc_map f);
	void (*print)(struct scpVector* vector, scpFunc_print print);
	void (*fprint)(struct scpVector* vector, FILE* stream, scpFunc_fprint fprint);
} const scpVector = {
	.new = scpVector_new,
	.delete = scpVector_delete,
	.clone = scpVector_clone,
	.fclone = scpVector_fclone,
	.copy = scpVector_copy,
	.fcopy = scpVector_fcopy,
	.resize = scpVector_resize,
	.reserve = scpVector_reserve,
	.at = scpVector_at,
	.append = scpVector_append,
	.pop = scpVector_pop,
	.map_index = scpVector_map_index,
	.map = scpVector_map,
	.print = scpVector_print,
	.fprint = scpVector_fprint
};

#ifdef SCP_IMPLEMENTATION

struct scpVector* scpVector_new(size_t count, size_t size) {
	struct scpVector* vector = (struct scpVector*)malloc(sizeof(struct scpVector));
	vector->type = &scpVector;
	vector->data = NULL;
	vector->count = 0;
	vector->size = size;
	vector->reserved = 0;
	scpVector_reserve(vector, count);
	return vector;
}

void scpVector_delete(struct scpVector* vector) {
	free(vector->data);
	free(vector);
}

struct scpVector* scpVector_clone(struct scpVector* vector) {
	struct scpVector* new_vector = (struct scpVector*)malloc(sizeof(struct scpVector));
	new_vector->type = &scpVector;
	new_vector->data = malloc(vector->reserved * vector->size);
	memcpy(new_vector->data, vector->data, vector->count * vector->size);
	new_vector->count = vector->count;
	new_vector->size = vector->size;
	new_vector->reserved = vector->reserved;
	return new_vector;
}

struct scpVector* scpVector_fclone(struct scpVector* vector, scpFunc_copy copy_data) {
	struct scpVector* new_vector = (struct scpVector*)malloc(sizeof(struct scpVector));
	new_vector->type = &scpVector;
	new_vector->data = malloc(vector->reserved * vector->size);
	for (size_t i = 0; i < vector->count * vector->size; i += vector->size)
		copy_data((char*)vector->data + i, (char*)new_vector->data + i);
	new_vector->count = vector->count;
	new_vector->size = vector->size;
	new_vector->reserved = vector->reserved;
	return new_vector;
}

void scpVector_copy(struct scpVector* vector, struct scpVector* new_vector) {
	new_vector->type = &scpVector;
	new_vector->data = malloc(vector->reserved * vector->size);
	memcpy(new_vector->data, vector->data, vector->count * vector->size);
	new_vector->count = vector->count;
	new_vector->size = vector->size;
	new_vector->reserved = vector->reserved;
}

void scpVector_fcopy(struct scpVector* vector, struct scpVector* new_vector, scpFunc_copy copy_data) {
	new_vector->type = &scpVector;
	new_vector->data = malloc(vector->reserved * vector->size);
	for (size_t i = 0; i < vector->count * vector->size; i += vector->size)
		copy_data((char*)vector->data + i, (char*)new_vector->data + i);
	new_vector->count = vector->count;
	new_vector->size = vector->size;
	new_vector->reserved = vector->reserved;
}

void scpVector_resize(struct scpVector* vector, size_t count) {
	if (count == vector->count) return;
	if (count < vector->reserved) {
		if (count < vector->count && count * 100 / vector->reserved < 10)
			scpVector_reserve(vector, count);
		vector->count = count;
		return;
	}
	vector->count = count;
	scpVector_reserve(vector, count);
}

void scpVector_reserve(struct scpVector* vector, size_t count) {
	while (count < vector->reserved)
		vector->reserved >>= 1;
	if (vector->reserved == 0)
		vector->reserved = 1;
	while (vector->reserved < count)
		vector->reserved <<= 1;
	if (vector->data)
		vector->data = realloc(vector->data, vector->reserved * vector->size);
	else
		vector->data = malloc(vector->reserved * vector->size);
}

void* scpVector_at(struct scpVector* vector, size_t index) {
	if (index >= vector->count)
		SCP_EXCEPTION(scpException_Exception, "index can't be greater or equal to count (%lu >= %lu)", index, vector->count);
	return (void*)((char*)vector->data + index * vector->size);
}

void* scpVector_append(struct scpVector* vector) {
	scpVector_resize(vector, vector->count + 1);
	return (void*)((char*)vector->data + (vector->count - 1) * vector->size);
}

void scpVector_pop(struct scpVector* vector) {
	if (vector->count == 0) SCP_EXCEPTION(scpException_OutOfBound, "can't pop an empty vector");
	scpVector_resize(vector, vector->count - 1);
}

void scpVector_map_index(struct scpVector* vector, scpFunc_map_index f) {
	for (size_t i = 0; i < vector->count; ++i)
		f((void*)((char*)vector->data + i * vector->size), i, vector->count);
}

void scpVector_map(struct scpVector* vector, scpFunc_map f) {
	for (size_t i = 0; i < vector->count; ++i)
		f((void*)((char*)vector->data + i * vector->size));
}

void scpVector_print(struct scpVector* vector, scpFunc_print print) {
	fputc('[', stdout);
	for (size_t i = 0; i < vector->count; ++i) {
		print((void*)((char*)vector->data + i * vector->size));
		if (i != vector->count - 1)
			fputs(", ", stdout);
	}
	fputc(']', stdout);
}

void scpVector_fprint(struct scpVector* vector, FILE* stream, scpFunc_fprint fprint) {
	fputc('[', stream);
	for (size_t i = 0; i < vector->count; ++i) {
		fprint(stream, (void*)((char*)vector->data + i * vector->size));
		if (i != vector->count - 1)
			fputs(", ", stream);
	}
	fputc(']', stream);
}

#endif // SCP_IMPLEMENTATION

#endif // SCP_VECTOR_H
