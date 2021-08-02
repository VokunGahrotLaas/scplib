#ifndef SCP_HASHMAP_H
#define SCP_HASHMAP_H

#include <stdlib.h>
#include <stdio.h>

#include "scp/exceptions.h"

typedef struct scpHashMapItem {
	char* key;
	char* value;
} scpHashMapItem;

static scpHashMapItem scpHashMap_deleted_item = { NULL, NULL };

static scpHashMapItem* scpHashMapItem_new(void* key, void* value);
static void scpHashMapItem_delete(scpHashMapItem* item);

struct scpHashMapType;

struct scpHashMap {
	struct scpHashMapType* type;
	size_t size;
	size_t count;
	scpHashMapItem** items;
	scpFunc_hash hash_a;
	scpFunc_hash hash_b;
	scpFunc_cmp cmp;
};

static size_t scpHashMap_get_hash(struct scpHashMap* hashmap, void* data, const size_t attempt);

struct scpHashMap* scpHashMap_new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
void scpHashMap_delete(struct scpHashMap* hashmap);
void scpHashMap_insert(struct scpHashMap* hashmap, void* key, void* value);
void scpHashMap_remove(struct scpHashMap* hashmap, void* key);
void* scpHashMap_search(struct scpHashMap* hashmap, void* key);

struct scpHashMapType {
	struct scpHashMap* (*new)(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
	void (*delete)(struct scpHashMap* hashmap);
	void (*insert)(struct scpHashMap* hashmap, void* key, void* value);
	void (*remove)(struct scpHashMap* hashmap, void* key);
	void* (*search)(struct scpHashMap* hashmap, void* key);
} scpHashMap = {
	.new = scpHashMap_new,
	.delete = scpHashMap_delete,
	.insert = scpHashMap_insert,
	.remove = scpHashMap_remove,
	.search = scpHashMap_search
};

static scpHashMapItem* scpHashMapItem_new(void* key, void* value) {
	scpHashMapItem* item = (scpHashMapItem*)malloc(sizeof(scpHashMapItem));
	item->key = key;
	item->value = value;
	return item;
}

static void scpHashMapItem_delete(scpHashMapItem* item) {
	free(item);
}

static size_t scpHashMap_get_hash(struct scpHashMap* hashmap, void* data, const size_t attempt) {
	const size_t hash_a = hashmap->hash_a(data) % hashmap->size;
	const size_t hash_b = hashmap->hash_b(data) % hashmap->size;
	return (hash_a + (attempt * (hash_b + 1))) % hashmap->size;
}

struct scpHashMap* scpHashMap_new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp) {
	struct scpHashMap* hashmap = (struct scpHashMap*)malloc(sizeof(struct scpHashMap));
	hashmap->type = &scpHashMap;
	hashmap->size = 53;
	hashmap->count = 0;
	hashmap->items = (scpHashMapItem**)calloc(hashmap->size, sizeof(scpHashMapItem*));
	hashmap->hash_a = hash_a;
	hashmap->hash_b = hash_b;
	hashmap->cmp = cmp;
	return hashmap;
}

void scpHashMap_delete(struct scpHashMap* hashmap) {
	free(hashmap);
}

void scpHashMap_insert(struct scpHashMap* hashmap, void* key, void* value) {
	scpHashMapItem* item = scpHashMapItem_new(key, value);
	size_t index = scpHashMap_get_hash(hashmap, item->key, 0);
	scpHashMapItem* cur_item = hashmap->items[index];
	for (size_t i = 1; cur_item != NULL && cur_item != &scpHashMap_deleted_item; ++i) {
		if (cur_item != &scpHashMap_deleted_item && hashmap->cmp(cur_item->key, key) == 0) {
			scpHashMapItem_delete(cur_item);
			hashmap->items[index] = item;
			return;
		}
		index = scpHashMap_get_hash(hashmap, item->key, i);
		cur_item = hashmap->items[index];
	}
	hashmap->items[index] = item;
	++hashmap->count;
}

void scpHashMap_remove(struct scpHashMap* hashmap, void* key) {
	size_t index = scpHashMap_get_hash(hashmap, key, 0);
	scpHashMapItem* item = hashmap->items[index];
	for (size_t i = 1; item != NULL; ++i) {
		if (item != &scpHashMap_deleted_item && hashmap->cmp(item->key, key) == 0) {
			scpHashMapItem_delete(item);
			hashmap->items[index] = &scpHashMap_deleted_item;
		}
		index = scpHashMap_get_hash(hashmap, key, i);
		item = hashmap->items[index];
	}
	--hashmap->count;
}

void* scpHashMap_search(struct scpHashMap* hashmap, void* key) {
	size_t index = scpHashMap_get_hash(hashmap, key, 0);
	scpHashMapItem* item = hashmap->items[index];
	for (size_t i = 1; item != NULL; ++i) {
		if (item != &scpHashMap_deleted_item && hashmap->cmp(item->key, key) == 0)
			return item->value;
		index = scpHashMap_get_hash(hashmap, key, i);
		item = hashmap->items[index];
	}
	return NULL;
}

#endif // SCP_HASHMAP_H