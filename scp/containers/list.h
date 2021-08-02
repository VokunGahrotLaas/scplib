#ifndef SCP_LIST_H
#define SCP_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "scp/utils/macros.h"

#ifndef SCP_PEDANTIC
#warning "don't use this for now, it's an old code, work in progress..."
#endif

typedef struct scpListNode {
	void* data;
	struct scpListNode* next;
} scpListNode;

typedef struct {
	scpListNode* first;
} scpList;

scpListNode* scpListNode_create(void* data);

scpList* scpList_create(void);
scpList* scpList_copy(scpList* other);
scpList* scpList_fcopy(scpList* other, scpFunc_clone clone_data);
void scpList_destroy(scpList* list);
size_t scpList_size(scpList* list);
void scpList_push_front(scpList* list, void* data);
void scpList_push_back(scpList* list, void* data);
void* scpList_pop_front(scpList* list);
void* scpList_pop_back(scpList* list);
void scpList_map_index(scpList* list, scpFunc_map_index f);
void scpList_map(scpList* list, scpFunc_map f);
void scpList_print(scpList* list, scpFunc_print print_element);

scpListNode* scpListNode_create(void* data) {
	scpListNode* node = (scpListNode*)malloc(sizeof(scpListNode));
	node->next = NULL;
	node->data = data;
	return node;
}

scpList* scpList_create(void) {
	scpList* list = (scpList*)malloc(sizeof(scpList));
	list->first = NULL;
	return list;
}

scpList* scpList_copy(scpList* other) {
	scpList* list = scpList_create();
	if (other->first == NULL) return list;

	list->first = scpListNode_create(other->first->data);
	scpListNode* node = list->first; scpListNode* other_node = other->first;
	for (; other_node->next != NULL; node = node->next, other_node = other_node->next)
		node->next = scpListNode_create(other_node->next->data);

	return list;
}

scpList* scpList_fcopy(scpList* other, scpFunc_clone clone_data) {
	scpList* list = scpList_create();
	if (other->first == NULL) return list;

	list->first = scpListNode_create(clone_data(other->first->data));
	scpListNode* node = list->first; scpListNode* other_node = other->first;
	for (; other_node->next != NULL; node = node->next, other_node = other_node->next)
		node->next = scpListNode_create(clone_data(other_node->next->data));

	return list;
}

void scpList_destroy(scpList* list) {
	scpListNode* node = list->first;
	while (node != NULL) {
		scpListNode* to_free = node;
		node = node->next;
		free(to_free);
	}
	free(list);
}

size_t scpList_size(scpList* list) {
	size_t i = 0;
	for (scpListNode* node = list->first; node != NULL; node = node->next, ++i);
	return i;
}

void scpList_push_front(scpList* list, void* data) {
	if (list->first == NULL) {
		list->first = scpListNode_create(data);
		return;
	}

	scpListNode* node = scpListNode_create(list->first->data);
	list->first->data = data;
	node->next = list->first->next;
	list->first->next = node;
}

void scpList_push_back(scpList* list, void* data) {
	if (list->first == NULL) {
		list->first = scpListNode_create(data);
		return;
	}

	scpListNode* node;
	for (node = list->first; node->next != NULL; node = node->next);
	node->next = scpListNode_create(data);
}

void* scpList_pop_front(scpList* list) {
	if (list->first == NULL) {
		fprintf(stderr, "scpList_pop_front: list was empty\n");
		exit(EXIT_FAILURE);
	}

	if (list->first->next == NULL) {
		void* data = list->first->data;
		free(list->first);
		list->first = NULL;
		return data;
	}

	scpListNode* old = list->first->next;
	void* data = list->first->data;
	list->first->data = old->data;
	list->first->next = old->next;
	old->next = NULL;
	free(old);
	return data;
}

void* scpList_pop_back(scpList* list) {
	if (list->first == NULL) {
		fprintf(stderr, "scpList_pop_back: list was empty\n");
		exit(EXIT_FAILURE);
	}

	if (list->first->next == NULL) {
		void* data = list->first->data;
		free(list->first);
		list->first = NULL;
		return data;
	}

	scpListNode* node;
	for (node = list->first; node->next->next != NULL; node = node->next);
	void* data = node->next->data;
	free(node->next);
	node->next = NULL;
	return data;
}

void scpList_map_index(scpList* list, scpFunc_map_index f) {
	size_t size = scpList_size(list);
	size_t i = 0;
	for (scpListNode* node = list->first; node != NULL; node = node->next)
		f(node->data, i++, size);
}

void scpList_map(scpList* list, scpFunc_map f) {
	for (scpListNode* node = list->first; node != NULL; node = node->next)
		f(node->data);
}

void scpList_print(scpList* list, scpFunc_print print_element) {
	fputc('[' ,stdout);
	for (scpListNode* node = list->first; node != NULL; node = node->next) {
		print_element(node->data);
		if (node->next != NULL)
			fputs(", ", stdout);
	}
	fputc(']', stdout);
}

#endif // SCP_LIST_H