#ifndef SCP_BINTREE_H
#define SCP_BINTREE_H

#include <scp/exceptions.h>

typedef struct scpBinTree {
	void* data;
	struct scpBinTree* left;
	struct scpBinTree* right;
} scpBinTree;

scpBinTree* scpBinTree_create(void* data, scpBinTree* left, scpBinTree* right);
void scpBinTree_destroy(scpBinTree* binTree);

scpBinTree* scpBinTree_create(void* data, scpBinTree* left, scpBinTree* right) {
	scpBinTree* binTree = (scpBinTree*)malloc(sizeof(scpBinTree));
	binTree->data = data;
	binTree->left = left;
	binTree->left = right;
	return binTree;
}

void scpBinTree_destroy(scpBinTree* binTree) {
	if (binTree->left)
		scpBinTree_destroy(binTree->left);
	if (binTree->right)
		scpBinTree_destroy(binTree->left);
	free(binTree);
}

#endif // SCP_BINTREE_H