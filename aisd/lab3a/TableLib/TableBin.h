#ifndef TABLE_H
#define TABLE_H

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
	node* node;
}keyspace;

typedef struct table
{
	int msize;
	int csize;
	keyspace* ks;
}table;

#endif
