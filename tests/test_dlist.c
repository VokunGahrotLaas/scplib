#include <stdlib.h>
#include <stdio.h>

#include "scp/containers/dlist.h"

static inline void print_size_t(void* data);
static inline void print_size_tDList(const char* name, scpDList* list);
static inline void* copy_size_t(void* other);
static inline void square(void* data);

int main(void) {
	size_t size = 10;

	size_t* data = calloc(size, sizeof(size_t));
	for (size_t i = 0; i < size; ++i)
		data[i] = size - i - 1;

	scpDList* list = scpDList_create();
	for (size_t i = 0; i < size; ++i)
		scpDList_push_front(list, data + i);

	scpDList* copy = scpDList_copy(list);
	scpDList* fcopy = scpDList_fcopy(list, copy_size_t);

	print_size_tDList("l", list);
	print_size_tDList("c", copy);
	print_size_tDList("f", fcopy);
	printf("\n");

	printf("popped: %zu %zu\n", *(size_t*)scpDList_pop_front(list), *(size_t*)scpDList_pop_back(list));

	size_t* data2 = malloc(sizeof(size_t));
	*data2 = data[0];
	scpDList_push_back(list, data2);
	printf("pushed %zu back\n", *data2);

	scpDList_map(list, square);
	printf("squared the list\n");
	printf("\n");

	print_size_tDList("l", list);
	print_size_tDList("c", copy);
	print_size_tDList("f", fcopy);
	printf("\n");

	while (fcopy->first != NULL) {
		free(scpDList_pop_front(fcopy));
		print_size_tDList("f", fcopy);
	}

	free(data);
	free(data2);

	scpDList_destroy(list);
	scpDList_destroy(copy);
	scpDList_destroy(fcopy);
	return EXIT_SUCCESS;
}

static inline void print_size_t(void* data) {
	printf("%zu", *(size_t*)data);
}

static inline void print_size_tDList(const char* name, scpDList* list) {
	printf("%s: ", name);
	scpDList_print(list, print_size_t);
	printf(" | ");
	scpDList_rprint(list, print_size_t);
	printf("\n");
}

static inline void* copy_size_t(void* other) {
	size_t* data = malloc(sizeof(size_t));
	*data = *(size_t*)other;
	return data;
}

static inline void square(void* data) {
	*(size_t*)data *= *(size_t*)data;
}