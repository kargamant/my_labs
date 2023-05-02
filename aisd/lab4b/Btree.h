#ifndef BTREE_H
#define BTREE_H

typedef struct Node
{
	int n; //amount of keys in array
	int* keys; //key array
	struct Node** child; //children array
	char** info; //information array
	struct Node* par; //parent pointer
}Node;

typedef struct Btree
{
	int t; //power of Btree
	Node* root;
}Btree;

typedef struct Item
{
	Node* node;
	struct Item* next;
}Item;

typedef enum ERR
{
	ERR_OK,
}ERR;

#endif
