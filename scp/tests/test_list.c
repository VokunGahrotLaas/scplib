#include <stdio.h>
#include <stdlib.h>

#include "../containers/list.h"

void print_int(void* data);
void print_intList(const char* name, scpList* list);
void* copy_int(void* other);
void square(void* data);



int main(int argc, const char** argv) {
	int size = 10;

	int* data = calloc(size, sizeof(int));
	for (int i = 0; i < size; ++i)
		data[i] = size - i - 1;

	scpList* list = scpList_create();
	for (int i = 0; i < size; ++i)
		scpList_push_front(list, data + i);

	scpList* copy = scpList_copy(list);
	scpList* fcopy = scpList_fcopy(list, copy_int);

	print_intList("l", list);
	print_intList("c", copy);
	print_intList("f", fcopy);
	printf("\n");

	printf("popped: %i %i\n", *(int*)scpList_pop_front(list), *(int*)scpList_pop_back(list));

	int* data2 = malloc(sizeof(int));
	*data2 = data[0];
	scpList_push_back(list, data2);
	printf("pushed %i back\n", *data2);

	scpList_map(list, square);
	printf("squared the list\n");
	printf("\n");

	print_intList("l", list);
	print_intList("c", copy);
	print_intList("f", fcopy);
	printf("\n");

	while (fcopy->first != NULL) {
		scpList_pop_front(fcopy);
		print_intList("f", fcopy);
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



void print_int(void* data) {
	printf("%i", *(int*)data);
}

void print_intList(const char* name, scpList* list) {
	printf("%s: ", name);
	scpList_print(list, print_int);
	printf("\n");
}

void* copy_int(void* other) {
	int* data = malloc(sizeof(int));
	*data = *(int*)other;
	return data;
}

void square(void* data) {
	*(int*)data = *(int*)data * *(int*)data;
}