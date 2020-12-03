#ifndef SCP_LIST_H
#define SCP_LIST_H

#include <stdio.h>
#include <stdlib.h>


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
scpList* scpList_fcopy(scpList* other, void*(*copy_data)(void*));
void scpList_destroy(scpList* list);
void scpList_push_front(scpList* list, void* data);
void scpList_push_back(scpList* list, void* data);
void* scpList_pop_front(scpList* list);
void* scpList_pop_back(scpList* list);
void scpList_map_node(scpList* list, void(*f)(scpListNode*));
void scpList_map(scpList* list, void(*f)(void*));
void scpList_print(scpList* list, void(*print_element)(void*));



scpListNode* scpListNode_create(void* data) {
	scpListNode* node = malloc(sizeof(scpListNode));
	node->next = NULL;
	node->data = data;
	return node;
}



scpList* scpList_create(void) {
	scpList* list = malloc(sizeof(scpList));
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

scpList* scpList_fcopy(scpList* other, void*(*copy_data)(void*)) {
	scpList* list = scpList_create();
	if (other->first == NULL) return list;

	list->first = scpListNode_create(copy_data(other->first->data));
	scpListNode* node = list->first; scpListNode* other_node = other->first;
	for (; other_node->next != NULL; node = node->next, other_node = other_node->next)
		node->next = scpListNode_create(copy_data(other_node->next->data));

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
		fputs("scpList_pop_front: list was empty\n", stderr);
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
		fputs("scpList_pop_back: list was empty\n", stderr);
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

void scpList_map_node(scpList* list, void(*f)(scpListNode*)) {
	for (scpListNode* node = list->first; node != NULL; node = node->next)
		f(node);
}

void scpList_map(scpList* list, void(*f)(void*)) {
	for (scpListNode* node = list->first; node != NULL; node = node->next)
		f(node->data);
}

void scpList_print(scpList* list, void(*print_element)(void*)) {
	void print(scpListNode* node) {
		print_element(node->data);
		if (node->next != NULL)
			fputs(", ", stdout);
	}

	fputc('[' ,stdout);
	scpList_map_node(list, print);
	fputc(']', stdout);
}

#endif // SCP_LIST_H