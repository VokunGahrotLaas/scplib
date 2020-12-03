#include <stdio.h>
#include <stdlib.h>

#include "../containers/dlist.h"

void print_int(void* data);
void print_intDList(const char* name, scpDList* list);
void* copy_int(void* other);
void square(void* data);



int main(int argc, const char** argv) {
	int size = 10;

	int* data = calloc(size, sizeof(int));
	for (int i = 0; i < size; ++i)
		data[i] = size - i - 1;

	scpDList* list = scpDList_create();
	for (int i = 0; i < size; ++i)
		scpDList_push_front(list, data + i);

	scpDList* copy = scpDList_copy(list);
	scpDList* fcopy = scpDList_fcopy(list, copy_int);

	print_intDList("l", list);
	print_intDList("c", copy);
	print_intDList("f", fcopy);
	printf("\n");

	printf("popped: %i %i\n", *(int*)scpDList_pop_front(list), *(int*)scpDList_pop_back(list));

	int* data2 = malloc(sizeof(int));
	*data2 = data[0];
	scpDList_push_back(list, data2);
	printf("pushed %i back\n", *data2);

	scpDList_map(list, square);
	printf("squared the list\n");
	printf("\n");

	print_intDList("l", list);
	print_intDList("c", copy);
	print_intDList("f", fcopy);
	printf("\n");

	while (fcopy->first != NULL) {
		free(scpDList_pop_front(fcopy));
		print_intDList("f", fcopy);
	}

	free(data);
	free(data2);

	scpDList_destroy(list);
	scpDList_destroy(copy);
	scpDList_destroy(fcopy);
	return EXIT_SUCCESS;
}



void print_int(void* data) {
	printf("%i", *(int*)data);
}

void print_intDList(const char* name, scpDList* list) {
	printf("%s: ", name);
	scpDList_print(list, print_int);
	printf(" | ");
	scpDList_rprint(list, print_int);
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