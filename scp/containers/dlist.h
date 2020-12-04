#ifndef SCP_DLIST_H
#define SCP_DLIST_H

#include <stdio.h>
#include <stdlib.h>


typedef struct scpDListNode {
	void* data;
	struct scpDListNode* prev;
	struct scpDListNode* next;
} scpDListNode;

typedef struct {
	scpDListNode* first;
	scpDListNode* last;
} scpDList;



scpDListNode* scpDListNode_create(void* data);

scpDList* scpDList_create(void);
scpDList* scpDList_copy(scpDList* other);
scpDList* scpDList_fcopy(scpDList* other, void*(*copy_data)(void*));
void scpDList_destroy(scpDList* list);
size_t scpDList_size(scpDList* list);
void scpDList_push_front(scpDList* list, void* data);
void scpDList_push_back(scpDList* list, void* data);
void* scpDList_pop_front(scpDList* list);
void* scpDList_pop_back(scpDList* list);
void scpDList_map_index(scpDList* list, void(*f)(void*, size_t, size_t));
void scpDList_rmap_index(scpDList* list, void(*f)(void*, size_t, size_t));
void scpDList_map(scpDList* list, void(*f)(void*));
void scpDList_rmap(scpDList* list, void(*f)(void*));
void scpDList_print(scpDList* list, void(*print_element)(void*));
void scpDList_rprint(scpDList* list, void(*print_element)(void*));



scpDListNode* scpDListNode_create(void* data) {
	scpDListNode* node = malloc(sizeof(scpDListNode));
	node->prev = NULL;
	node->next = NULL;
	node->data = data;
	return node;
}



scpDList* scpDList_create(void) {
	scpDList* list = malloc(sizeof(scpDList));
	list->first = NULL;
	return list;
}

scpDList* scpDList_copy(scpDList* other) {
	scpDList* list = scpDList_create();
	if (other->first == NULL) return list;

	list->first = scpDListNode_create(other->first->data);
	scpDListNode* node = list->first; scpDListNode* other_node = other->first;
	for (; other_node->next != NULL; node = node->next, other_node = other_node->next) {
		node->next = scpDListNode_create(other_node->next->data);
		node->next->prev = node;
	}

	list->last = node;

	return list;
}

scpDList* scpDList_fcopy(scpDList* other, void*(*copy_data)(void*)) {
	scpDList* list = scpDList_create();
	if (other->first == NULL) return list;

	list->first = scpDListNode_create(copy_data(other->first->data));
	scpDListNode* node = list->first; scpDListNode* other_node = other->first;
	for (; other_node->next != NULL; node = node->next, other_node = other_node->next) {
		node->next = scpDListNode_create(copy_data(other_node->next->data));
		node->next->prev = node;
	}

	list->last = node;

	return list;
}

void scpDList_destroy(scpDList* list) {
	scpDListNode* node = list->first;
	while (node != NULL) {
		scpDListNode* to_free = node;
		node = node->next;
		free(to_free);
	}
	free(list);
}

size_t scpDList_size(scpDList* list) {
	size_t i = 0;
	for (scpDListNode* node = list->first; node != NULL; node = node->next, ++i);
	return i;
}

void scpDList_push_front(scpDList* list, void* data) {
	if (list->first == NULL) {
		list->first = scpDListNode_create(data);
		list->last = list->first;
		return;
	}

	if (list->first == list->last) {
		list->first = scpDListNode_create(data);
		list->first->next = list->last;
		list->last->prev = list->first;
		return;
	}

	scpDListNode* node = scpDListNode_create(list->first->data);
	list->first->data = data;
	node->next = list->first->next;
	if (node->next != NULL)
		node->next->prev = node;
	node->prev = list->first;
	list->first->next = node;
}

void scpDList_push_back(scpDList* list, void* data) {
	if (list->last == NULL) {
		list->last = scpDListNode_create(data);
		list->first = list->last;
		return;
	}

	if (list->last == list->first) {
		list->last = scpDListNode_create(data);
		list->last->prev = list->first;
		list->first->next = list->last;
		return;
	}

	scpDListNode* node = scpDListNode_create(list->last->data);
	list->last->data = data;
	node->prev = list->last->prev;
	if (node->prev != NULL)
		node->prev->next = node;
	node->next = list->last;
	list->last->prev = node;
}

void* scpDList_pop_front(scpDList* list) {
	if (list->first == NULL) {
		fprintf(stderr, "scpDList_pop_front: list was empty\n");
		exit(EXIT_FAILURE);
	}

	if (list->first->next == NULL) {
		void* data = list->first->data;
		free(list->first);
		list->first = NULL;
		list->last = NULL;
		return data;
	}

	if (list->first->next == list->last)
		list->last = list->first;

	scpDListNode* old = list->first->next;
	void* data = list->first->data;
	list->first->data = old->data;
	list->first->next = old->next;
	if (list->first->next != NULL)
		list->first->next->prev = list->first;
	free(old);
	return data;
}

void* scpDList_pop_back(scpDList* list) {
	if (list->last == NULL) {
		fprintf(stderr, "scpDList_pop_back: list was empty\n");
		exit(EXIT_FAILURE);
	}

	if (list->last->prev == NULL) {
		void* data = list->last->data;
		free(list->last);
		list->last = NULL;
		list->first = NULL;
		return data;
	}

	if (list->last->prev == list->first)
		list->first = list->last;

	scpDListNode* old = list->last->prev;
	void* data = list->last->data;
	list->last->data = old->data;
	list->last->prev = old->prev;
	if (list->last->prev != NULL)
		list->last->prev->next = list->last;
	free(old);
	return data;
}

void scpDList_map_index(scpDList* list, void(*f)(void*, size_t, size_t)) {
	size_t size = scpDList_size(list);
	size_t i = 0;
	for (scpDListNode* node = list->first; node != NULL; node = node->next)
		f(node->data, i++, size);
}

void scpDList_rmap_index(scpDList* list, void(*f)(void*, size_t, size_t)) {
	size_t size = scpDList_size(list);
	size_t i = 0;
	for (scpDListNode* node = list->last; node != NULL; node = node->prev)
		f(node->data, size - i++ - 1, size);
}

void scpDList_map(scpDList* list, void(*f)(void*)) {
	for (scpDListNode* node = list->first; node != NULL; node = node->next)
		f(node->data);
}

void scpDList_rmap(scpDList* list, void(*f)(void*)) {
	for (scpDListNode* node = list->last; node != NULL; node = node->prev)
		f(node->data);
}

void scpDList_print(scpDList* list, void(*print_element)(void*)) {
	void print(void* data, size_t index, size_t size) {
		print_element(data);
		if (index != size - 1)
			fputs(", ", stdout);
	}

	fputc('[' ,stdout);
	scpDList_map_index(list, print);
	fputc(']', stdout);
}

void scpDList_rprint(scpDList* list, void(*print_element)(void*)) {
	void print(void* data, size_t index, size_t size) {
		print_element(data);
		if (index != 0)
			fputs(", ", stdout);
	}

	fputc('[' ,stdout);
	scpDList_rmap_index(list, print);
	fputc(']', stdout);
}

#endif // SCP_DLIST_H