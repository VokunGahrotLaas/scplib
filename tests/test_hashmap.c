#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/containers/hashmap.h"
#include "scp/utils.h"

int main(void) {
	struct scpHashMap* hashmap = scpHashMap.new(scpHash_stringA, scpHash_stringB, scpCmp_string);

	const char* keys[] = { "hi", "salut", "truc" };
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	printf("replaced ? %s\n", scpHashMap.insert(hashmap, keys[0], values + 2) ? "true" : "false");
	printf("result for '%s': %i\n", keys[0], *(int*)scpHashMap.search(hashmap, keys[0]));
	printf("\n");

	printf("replaced ? %s\n", scpHashMap.insert(hashmap, keys[0], values + 5) ? "true" : "false");
	printf("result for '%s': %i\n", keys[0], *(int*)scpHashMap.search(hashmap, keys[0]));
	printf("\n");

	printf("removed ? %s\n", scpHashMap.remove(hashmap, keys[0]) ? "true" : "false");
	printf("result for '%s': %p\n", keys[0], scpHashMap.search(hashmap, keys[0]));
	printf("\n");

	printf("removed ? %s\n", scpHashMap.remove(hashmap, keys[0]) ? "true" : "false");
	printf("result for '%s': %p\n", keys[0], scpHashMap.search(hashmap, keys[0]));
	printf("\n");

	scpHashMap.delete(hashmap);
	return EXIT_SUCCESS;
}
