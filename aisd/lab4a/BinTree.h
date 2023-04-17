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
int Traversing(Node* root, int key);
Node* Max(Node* root);
Node* Min(Node* root);
Node* Search(Node* root, int key);
int AddNode(Node* root, int key, char* info);
int DelNode(Node* root, int key);
void erased(Node* root);
int fimport(Node* root, char* fn);
int show(Node* root);
void out_node(Node* x, int spaces);

//error codes
typedef enum ERR
{
	ERR_OK,
	ERR_DUPL,
	ERR_NF,
	ERR_EOF,
	ERR_EMPTY,
}ERR;

typedef enum states
{
	FULL_TREE=-1,
}states;
#endif
