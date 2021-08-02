#ifndef SCP_HASHMAP_H
#define SCP_HASHMAP_H

#include <stdlib.h>
#include <stdio.h>

#include "scp/maths/primes.h"

#define scpHashMap_BASE_SIZE 50

typedef struct scpHashMapItem {
	const void* key;
	void* value;
} scpHashMapItem;

static scpHashMapItem scpHashMap_DELETED_ITEM = { .key = NULL, .value = NULL };

static scpHashMapItem* scpHashMapItem_new(const void* key, void* value);
static void scpHashMapItem_delete(scpHashMapItem* item);

struct scpHashMapType;

struct scpHashMap {
	struct scpHashMapType* type;
	size_t base_size;
	size_t size;
	size_t count;
	scpHashMapItem** items;
	scpFunc_hash hash_a;
	scpFunc_hash hash_b;
	scpFunc_cmp cmp;
};

static size_t scpHashMap_get_hash(struct scpHashMap* hashmap, const void* data, const size_t attempt);
static struct scpHashMap* scpHashMap_new_sized(const size_t base_size, scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
static void scpHashMap_resize(struct scpHashMap* hashmap, const size_t base_size);
static void scpHashMap_resize_up(struct scpHashMap* hashmap);
static void scpHashMap_resize_down(struct scpHashMap* hashmap);

struct scpHashMap* scpHashMap_new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
void scpHashMap_delete(struct scpHashMap* hashmap);
bool scpHashMap_insert(struct scpHashMap* hashmap, const void* key, void* value);
bool scpHashMap_remove(struct scpHashMap* hashmap, const void* key);
void* scpHashMap_search(struct scpHashMap* hashmap, const void* key);

struct scpHashMapType {
	struct scpHashMap* (*new)(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
	void (*delete)(struct scpHashMap* hashmap);
	bool (*insert)(struct scpHashMap* hashmap, const void* key, void* value);
	bool (*remove)(struct scpHashMap* hashmap, const void* key);
	void* (*search)(struct scpHashMap* hashmap, const void* key);
} scpHashMap = {
	.new = scpHashMap_new,
	.delete = scpHashMap_delete,
	.insert = scpHashMap_insert,
	.remove = scpHashMap_remove,
	.search = scpHashMap_search
};

static scpHashMapItem* scpHashMapItem_new(const void* key, void* value) {
	scpHashMapItem* item = (scpHashMapItem*)malloc(sizeof(scpHashMapItem));
	item->key = key;
	item->value = value;
	return item;
}

static void scpHashMapItem_delete(scpHashMapItem* item) {
	free(item);
}

static size_t scpHashMap_get_hash(struct scpHashMap* hashmap, const void* data, const size_t attempt) {
	const size_t hash_a = hashmap->hash_a(data) % hashmap->size;
	const size_t hash_b = hashmap->hash_b(data) % hashmap->size;
	return (hash_a + (attempt * (hash_b + 1))) % hashmap->size;
}

static struct scpHashMap* scpHashMap_new_sized(const size_t base_size, scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp) {
	struct scpHashMap* hashmap = (struct scpHashMap*)malloc(sizeof(struct scpHashMap));
	hashmap->type = &scpHashMap;
	hashmap->base_size = base_size;
	hashmap->size = (size_t)scpPrimes_next(hashmap->base_size);
	hashmap->count = 0;
	hashmap->items = (scpHashMapItem**)calloc(hashmap->size, sizeof(scpHashMapItem*));
	hashmap->hash_a = hash_a;
	hashmap->hash_b = hash_b;
	hashmap->cmp = cmp;
	return hashmap;
}

static void scpHashMap_resize(struct scpHashMap* hashmap, const size_t base_size) {
	if (base_size < scpHashMap_BASE_SIZE) return;

	struct scpHashMap* new_hashmap = scpHashMap_new_sized(base_size, hashmap->hash_a, hashmap->hash_b, hashmap->cmp);

	for (size_t i = 0; i < hashmap->size; ++i) {
		scpHashMapItem* item = hashmap->items[i];
		if (item != NULL && item != &scpHashMap_DELETED_ITEM)
			scpHashMap_insert(new_hashmap, item->key, item->value);
	}

	hashmap->base_size = base_size;
	SCP_SWAP(size_t, hashmap->size, new_hashmap->size)
	SCP_SWAP(scpHashMapItem**, hashmap->items, new_hashmap->items)

	scpHashMap_delete(new_hashmap);
}

static void scpHashMap_resize_up(struct scpHashMap* hashmap) {
	const size_t new_size = hashmap->base_size * 2;
	scpHashMap_resize(hashmap, new_size);
}

static void scpHashMap_resize_down(struct scpHashMap* hashmap) {
	const size_t new_size = hashmap->base_size / 2;
	scpHashMap_resize(hashmap, new_size);
}

struct scpHashMap* scpHashMap_new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp) {
	return scpHashMap_new_sized(scpHashMap_BASE_SIZE, hash_a, hash_b, cmp);
}

void scpHashMap_delete(struct scpHashMap* hashmap) {
	free(hashmap);
}

bool scpHashMap_insert(struct scpHashMap* hashmap, const void* key, void* value) {
	if (hashmap->count * 100 / hashmap->size > 70)
		scpHashMap_resize_up(hashmap);
	scpHashMapItem* item = scpHashMapItem_new(key, value);
	size_t index = scpHashMap_get_hash(hashmap, item->key, 0);
	scpHashMapItem* cur_item = hashmap->items[index];
	for (size_t i = 1; cur_item != NULL && cur_item != &scpHashMap_DELETED_ITEM; ++i) {
		if (cur_item != &scpHashMap_DELETED_ITEM && hashmap->cmp(cur_item->key, key) == 0) {
			scpHashMapItem_delete(cur_item);
			hashmap->items[index] = item;
			return true;
		}
		index = scpHashMap_get_hash(hashmap, item->key, i);
		cur_item = hashmap->items[index];
	}
	hashmap->items[index] = item;
	++hashmap->count;
	return false;
}

bool scpHashMap_remove(struct scpHashMap* hashmap, const void* key) {
	if (hashmap->count * 100 / hashmap->size < 10)
		scpHashMap_resize_down(hashmap);
	size_t index = scpHashMap_get_hash(hashmap, key, 0);
	scpHashMapItem* item = hashmap->items[index];
	for (size_t i = 1; item != NULL; ++i) {
		if (item != &scpHashMap_DELETED_ITEM && hashmap->cmp(item->key, key) == 0) {
			scpHashMapItem_delete(item);
			hashmap->items[index] = &scpHashMap_DELETED_ITEM;
			--hashmap->count;
			return true;
		}
		index = scpHashMap_get_hash(hashmap, key, i);
		item = hashmap->items[index];
	}
	return false;
}

void* scpHashMap_search(struct scpHashMap* hashmap, const void* key) {
	size_t index = scpHashMap_get_hash(hashmap, key, 0);
	scpHashMapItem* item = hashmap->items[index];
	for (size_t i = 1; item != NULL; ++i) {
		if (item != &scpHashMap_DELETED_ITEM && hashmap->cmp(item->key, key) == 0)
			return item->value;
		index = scpHashMap_get_hash(hashmap, key, i);
		item = hashmap->items[index];
	}
	return NULL;
}

#endif // SCP_HASHMAP_H