#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/containers/hashset.h"
#include "scp/utils/hash.h"
#include "scp/utils/cmp.h"

int main(void) {
	struct scpHashSet* hashmap = scpHashSet.new(scpHash_stringA, scpHash_stringB, scpCmp_string);

	const char* keys[] = { "hi", "salut", "truc" };

	printf("replaced ? %s\n", scpHashSet.insert(hashmap, keys[0]) ? "true" : "false");
	printf("result for '%s': %s\n", keys[0], scpHashSet.search(hashmap, keys[0]) ? "true" : "false");
	printf("\n");

	printf("replaced ? %s\n", scpHashSet.insert(hashmap, keys[0]) ? "true" : "false");
	printf("result for '%s': %s\n", keys[0], scpHashSet.search(hashmap, keys[0]) ? "true" : "false");
	printf("\n");

	printf("removed ? %s\n", scpHashSet_remove(hashmap, keys[0]) ? "true" : "false");
	printf("result for '%s': %s\n", keys[0], scpHashSet.search(hashmap, keys[0]) ? "true" : "false");
	printf("\n");

	printf("removed ? %s\n", scpHashSet_remove(hashmap, keys[0]) ? "true" : "false");
	printf("result for '%s': %s\n", keys[0], scpHashSet.search(hashmap, keys[0]) ? "true" : "false");
	printf("\n");

	scpHashSet.delete(hashmap);
	return EXIT_SUCCESS;
}
