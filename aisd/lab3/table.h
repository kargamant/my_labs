#ifndef TABLE_H
#define TABLE_H
#include "list.h"

typedef struct KeySpace
{
	int key;
	Node* node;
}KeySpace;

typedef struct Table
{
	int msize;
	KeySpace* ks;
}Table;


typedef struct Node
{
	int relo;
	Item* data;
	Node* next;
}Node;

typedef struct Item
{
	char* data;
}Item;
#endif
