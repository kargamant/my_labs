#ifndef TABLEBIN_H
#define TABLEBIN_H
#include <stdio.h>

typedef struct node
{
	int rel;
	int offset;
	int len;
	struct node* next;
}node;

typedef struct keyspace
{
	unsigned int key;
	node* Node;
}keyspace;

typedef struct table
{
	int msize;
	int csize;
	char* fd;
	FILE* fi;
	keyspace* ks;
}table;

#endif
