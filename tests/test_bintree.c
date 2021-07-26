#include <stdlib.h>
#include <stdio.h>

#include "scp/containers/bintree.h"

static inline void print_size_t(void* data);
static inline void square(void* data);

int main(void) {
	scpBinTree* binTree = scpBinTree_create(NULL, NULL, NULL);
	scpBinTree_destroy(binTree);
	return EXIT_SUCCESS;
}

static inline void print_size_t(void* data) {
	printf("%lu", *(size_t*)data);
}

static inline void square(void* data) {
	*(size_t*)data *= *(size_t*)data;
}