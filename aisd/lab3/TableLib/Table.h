#ifndef TABLE_H
#define TABLE_H
#include <stdio.h>

typedef struct Item
{
	char* data;
	struct KeySpace* ks;
}Item;

typedef struct Node
{
	int rel;
	Item* item;
	struct Node* next;
}Node;

typedef struct KeySpace
{
	unsigned int key;
	Node* node;
}KeySpace;

typedef struct Table
{
	int msize;
	int csize;
	KeySpace* ks;
}Table;

Table* input(FILE* fd);
void erase(Table* t);
void output(Table* t);


#endif
