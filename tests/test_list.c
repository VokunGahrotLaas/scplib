#include <stdlib.h>
#include <stdio.h>

#include "scp/containers/list.h"

#include "utils.h"

static void print_size_tList(const char* name, scpList* list);

int main(void) {
	size_t size = 10;

	size_t* data = calloc(size, sizeof(size_t));
	for (size_t i = 0; i < size; ++i)
		data[i] = size - i - 1;

	scpList* list = scpList_create();
	for (size_t i = 0; i < size; ++i)
		scpList_push_front(list, data + i);

	scpList* copy = scpList_copy(list);
	scpList* fcopy = scpList_fcopy(list, scpClone_size);

	print_size_tList("l", list);
	print_size_tList("c", copy);
	print_size_tList("f", fcopy);
	printf("\n");

	printf("popped: %zu %zu\n", *(size_t*)scpList_pop_front(list), *(size_t*)scpList_pop_back(list));

	size_t* data2 = malloc(sizeof(size_t));
	*data2 = data[0];
	scpList_push_back(list, data2);
	printf("pushed %zu back\n", *data2);

	scpList_map(list, square_size_t);
	printf("squared the list\n");
	printf("\n");

	print_size_tList("l", list);
	print_size_tList("c", copy);
	print_size_tList("f", fcopy);
	printf("\n");

	while (fcopy->first != NULL) {
		scpList_pop_front(fcopy);
		print_size_tList("f", fcopy);
	}

	free(data);
	free(data2);
	for (scpListNode* node = fcopy->first; node != NULL; node = node->next)
		free(node->data);

	scpList_destroy(list);
	scpList_destroy(copy);
	scpList_destroy(fcopy);
	return EXIT_SUCCESS;
}

static void print_size_tList(const char* name, scpList* list) {
	printf("%s: ", name);
	scpList_print(list, scpPrint_size);
	printf("\n");
}
