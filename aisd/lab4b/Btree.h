#ifndef BTREE_H
#define BTREE_H

typedef struct Item
{
	char* data;
	struct Item* next;
}Item;

typedef struct Node
{
	int mark; //if a node was allocated in split operation than it will be marked to be freed saperately
	int n; //amount of keys in array
	int* keys; //key array
	struct Node** child; //children array
	Item* info; //information array
	struct Node* par; //parent pointer
}Node;

typedef struct Btree
{
	int t; //power of Btree
	Node* root;
}Btree;


Node* Min(Btree* tr, int key);
void erase(Btree* tr);
int Traversing(Btree* tr, int key);
Btree* InitBtree(int t);
Item* Search(Btree* tr, int key);
int AddNode(Btree* tr, int key, char* info);
Node* Split(Btree* tr, Node* x, int i);
void show(Btree* tr, int level);
void fimport(Btree* tr, char* fn);
int DelNode(Btree* tr, int key, int rel);
void MergeNode(Node* y, Node* z, Btree* tr);


typedef enum ERR
{
	ERR_OK, 
	ERR_NF, //If we haven't found element of this number in key while deleting
	ERR_IR, //If there is only one element of this key, but incorrect release number entered
}ERR;

#endif
