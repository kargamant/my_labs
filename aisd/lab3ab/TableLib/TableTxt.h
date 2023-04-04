#ifndef TABLETXT_H
#define TABLETXT_H

typedef struct item
{
	char* data;
	struct keyspace* ks;
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

#endif
