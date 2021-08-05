#include <stdlib.h>
#include <stdio.h>

#include "scp/containers/bintree.h"

#include "utils.h"

int main(void) {
	scpBinTree* binTree = scpBinTree_create(NULL, NULL, NULL);
	scpBinTree_destroy(binTree);
	return EXIT_SUCCESS;
}
