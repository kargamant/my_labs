#ifndef TABLEBIN_H
#define TABLEBIN_H

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
	char* fi;
	keyspace* ks;
}table;

#endif
