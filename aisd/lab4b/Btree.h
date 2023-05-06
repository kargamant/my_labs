#ifndef BTREE_H
#define BTREE_H

typedef struct Item
{
	char* data;
	struct Item* next;
}Item;

typedef struct Node
{
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


Node* Min(Btree* tr);
int Traversing(Btree* tr, int key);
Btree* InitBtree(int t);
Item* Search(Btree* tr, int key);
int AddNode(Btree* tr, int key, char* info);
void Split(Btree* tr, Node* x, int i);
void show(Btree* tr, int level);
void fimport(Btree* tr, char* fn);


typedef enum ERR
{
	ERR_OK,
}ERR;

#endif
