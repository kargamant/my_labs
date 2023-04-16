#ifndef BIN_TREE_H
#define BIN_TREE_H

typedef struct Node
{
	int key;
	char* info;
	struct Node* left;
	struct Node* right;
	struct Node* prev;
}Node;

void Traversing(Node* tr, int key);
#endif
