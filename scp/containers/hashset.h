#ifndef SCP_HASHSET_H
#define SCP_HASHSET_H

#include <stdlib.h>
#include <stdio.h>

#include "scp/maths/primes.h"

#define scpHashSet_BASE_SIZE 50

typedef struct scpHashSetItem {
	const void* key;
} scpHashSetItem;

static scpHashSetItem scpHashSet_DELETED_ITEM = { .key = NULL };

scpAttribute_malloc static scpHashSetItem* scpHashSetItem_new(const void* key);
static void scpHashSetItem_delete(scpHashSetItem* item);

struct scpHashSetType;

struct scpHashSet {
	struct scpHashSetType* type;
	size_t base_size;
	size_t size;
	size_t count;
	scpHashSetItem** items;
	scpFunc_hash hash_a;
	scpFunc_hash hash_b;
	scpFunc_cmp cmp;
};

static size_t scpHashSet_get_hash(struct scpHashSet* hashmap, const void* data, const size_t attempt);
scpAttribute_malloc static struct scpHashSet* scpHashSet_new_sized(const size_t base_size, scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
static void scpHashSet_resize(struct scpHashSet* hashmap, const size_t base_size);
static void scpHashSet_resize_up(struct scpHashSet* hashmap);
static void scpHashSet_resize_down(struct scpHashSet* hashmap);

scpAttribute_malloc struct scpHashSet* scpHashSet_new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
void scpHashSet_delete(struct scpHashSet* hashmap);
bool scpHashSet_insert(struct scpHashSet* hashmap, const void* key);
bool scpHashSet_remove(struct scpHashSet* hashmap, const void* key);
bool scpHashSet_search(struct scpHashSet* hashmap, const void* key);

struct scpHashSetType {
	struct scpHashSet* (*new)(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp);
	void (*delete)(struct scpHashSet* hashmap);
	bool (*insert)(struct scpHashSet* hashmap, const void* key);
	bool (*remove)(struct scpHashSet* hashmap, const void* key);
	bool (*search)(struct scpHashSet* hashmap, const void* key);
} scpHashSet = {
	.new = scpHashSet_new,
	.delete = scpHashSet_delete,
	.insert = scpHashSet_insert,
	.remove = scpHashSet_remove,
	.search = scpHashSet_search
};

static scpHashSetItem* scpHashSetItem_new(const void* key) {
	scpHashSetItem* item = (scpHashSetItem*)malloc(sizeof(scpHashSetItem));
	item->key = key;
	return item;
}

static void scpHashSetItem_delete(scpHashSetItem* item) {
	free(item);
}

static size_t scpHashSet_get_hash(struct scpHashSet* hashmap, const void* data, const size_t attempt) {
	const size_t hash_a = hashmap->hash_a(data) % hashmap->size;
	const size_t hash_b = hashmap->hash_b(data) % hashmap->size;
	return (hash_a + (attempt * (hash_b + 1))) % hashmap->size;
}

static struct scpHashSet* scpHashSet_new_sized(const size_t base_size, scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp) {
	struct scpHashSet* hashmap = (struct scpHashSet*)malloc(sizeof(struct scpHashSet));
	hashmap->type = &scpHashSet;
	hashmap->base_size = base_size;
	hashmap->size = (size_t)scpPrimes_next(hashmap->base_size);
	hashmap->count = 0;
	hashmap->items = (scpHashSetItem**)calloc(hashmap->size, sizeof(scpHashSetItem*));
	hashmap->hash_a = hash_a;
	hashmap->hash_b = hash_b;
	hashmap->cmp = cmp;
	return hashmap;
}

static void scpHashSet_resize(struct scpHashSet* hashmap, const size_t base_size) {
	if (base_size < scpHashSet_BASE_SIZE) return;

	struct scpHashSet* new_hashmap = scpHashSet_new_sized(base_size, hashmap->hash_a, hashmap->hash_b, hashmap->cmp);

	for (size_t i = 0; i < hashmap->size; ++i) {
		scpHashSetItem* item = hashmap->items[i];
		if (item != NULL && item != &scpHashSet_DELETED_ITEM)
			scpHashSet_insert(new_hashmap, item->key);
	}

	hashmap->base_size = base_size;
	SCP_SWAP(size_t, hashmap->size, new_hashmap->size)
	SCP_SWAP(scpHashSetItem**, hashmap->items, new_hashmap->items)

	scpHashSet_delete(new_hashmap);
}

static void scpHashSet_resize_up(struct scpHashSet* hashmap) {
	const size_t new_size = hashmap->base_size * 2;
	scpHashSet_resize(hashmap, new_size);
}

static void scpHashSet_resize_down(struct scpHashSet* hashmap) {
	const size_t new_size = hashmap->base_size / 2;
	scpHashSet_resize(hashmap, new_size);
}

struct scpHashSet* scpHashSet_new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp) {
	return scpHashSet_new_sized(scpHashSet_BASE_SIZE, hash_a, hash_b, cmp);
}

void scpHashSet_delete(struct scpHashSet* hashmap) {
	free(hashmap);
}

bool scpHashSet_insert(struct scpHashSet* hashmap, const void* key) {
	if (hashmap->count * 100 / hashmap->size > 70)
		scpHashSet_resize_up(hashmap);
	scpHashSetItem* item = scpHashSetItem_new(key);
	size_t index = scpHashSet_get_hash(hashmap, item->key, 0);
	scpHashSetItem* cur_item = hashmap->items[index];
	for (size_t i = 1; cur_item != NULL && cur_item != &scpHashSet_DELETED_ITEM; ++i) {
		if (cur_item != &scpHashSet_DELETED_ITEM && hashmap->cmp(cur_item->key, key) == 0) {
			scpHashSetItem_delete(cur_item);
			hashmap->items[index] = item;
			return true;
		}
		index = scpHashSet_get_hash(hashmap, item->key, i);
		cur_item = hashmap->items[index];
	}
	hashmap->items[index] = item;
	++hashmap->count;
	return false;
}

bool scpHashSet_remove(struct scpHashSet* hashmap, const void* key) {
	if (hashmap->count * 100 / hashmap->size < 10)
		scpHashSet_resize_down(hashmap);
	size_t index = scpHashSet_get_hash(hashmap, key, 0);
	scpHashSetItem* item = hashmap->items[index];
	for (size_t i = 1; item != NULL; ++i) {
		if (item != &scpHashSet_DELETED_ITEM && hashmap->cmp(item->key, key) == 0) {
			scpHashSetItem_delete(item);
			hashmap->items[index] = &scpHashSet_DELETED_ITEM;
			--hashmap->count;
			return true;
		}
		index = scpHashSet_get_hash(hashmap, key, i);
		item = hashmap->items[index];
	}
	return false;
}

bool scpHashSet_search(struct scpHashSet* hashmap, const void* key) {
	size_t index = scpHashSet_get_hash(hashmap, key, 0);
	scpHashSetItem* item = hashmap->items[index];
	for (size_t i = 1; item != NULL; ++i) {
		if (item != &scpHashSet_DELETED_ITEM && hashmap->cmp(item->key, key) == 0)
			return true;
		index = scpHashSet_get_hash(hashmap, key, i);
		item = hashmap->items[index];
	}
	return false;
}

#endif // SCP_HASHSET_H