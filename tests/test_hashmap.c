#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/containers/hashmap.h"
#include "scp/hash.h"

int cmp_string(const void* a, const void* b) {
	return strcmp((const char*)a, (const char*)b);
}

int main(void) {
	struct scpHashMap* hashmap = scpHashMap.new(scpHash_stringA, scpHash_stringB, cmp_string);

	char* key = "hi";
	char* value = "salut";
	scpHashMap.insert(hashmap, key, value);
	printf("search for '%s': '%s'\n", key, (char*)scpHashMap.search(hashmap, key));

	scpHashMap_remove(hashmap, key);
	printf("search for '%s': '%s'\n", key, (char*)scpHashMap.search(hashmap, key));

	scpHashMap.delete(hashmap);
	return EXIT_SUCCESS;
}
