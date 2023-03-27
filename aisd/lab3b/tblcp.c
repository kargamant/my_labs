#include <stdio.h>
#include <stdlib.h>

typedef struct item
{
	char* data;
	struct keyspace* ks;
	int offset;
	int len;
}item;

typedef struct node
{
	int rel;
	item* item;
	struct node* next;
}node;

typedef struct keyspace
{
	unsigned int key;
	node* node;
}keyspace;

typedef struct table
{
	int msize;
	int csize;
	keyspace* ks;
}table;
#include "Table.h"

//data file: msize|key1|N1|rel|offset|len|rel|offset|len...|key2|N2|rel|offset|len...ROL
//info file: info1|info2|info3|info4...
int init(int msize, char* fn)
{
	FILE* fd=fopen(fn, "r+b");
	if(!fd) return ERR_FIL;
	fwrite(&msize, sizeof(int), 1, fd);
	fclose(fd);
	return ERR_OK;
}

int readt(Table* t, char* fnd, char* fni)
{
	FILE* data=fopen(fnd, "r+b");
	FILE* info=fopen(fni, "r+b");
	if(!data || !info) return ERR_FIL;
	
}
