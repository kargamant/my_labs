#ifndef BIN_TREE_H
#define BIN_TREE_H

//structure
typedef struct Node
{
	int key;
	char* info;
	struct Node* left;
	struct Node* right;
	struct Node* prev;
}Node;

//methods
void Traversing(Node* tr, int key);
Node* Max(Node* root);
Node* Min(Node* root);
Node* Search(Node* root, int key);
int AddNode(Node* root, int key, char* info);
int DelNode(Node* root, int key);
void erased(Node* root);
int fimport(Node* root, char* fn);
//error codes
#endif
