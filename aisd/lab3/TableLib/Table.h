#ifndef TABLE_H
#define TABLE_H
#include <stdio.h>

//structures
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

//methods
Table* input(FILE* fd);
void erased(Table* t);
void output(Table* t);
void outputks(KeySpace* ptr);
void outputnd(Node* gr);
int DelByKey(Table* t, int key);
int DelByVersion(Table* t, int key, int rel);
int add(Table* t, int key, char* c);
KeySpace* SearchByKey(Table* t, int key);
Node* SearchByVersion(Table* t, int key, int rel);

//error codes constants
typedef enum ERR
{
	ERR_OK,
	ERR_EMPTY,
	ERR_FULL,
	ERR_NO_FOUND,
	ERR_DUP
}ERR;

#endif
