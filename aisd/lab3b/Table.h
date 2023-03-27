#ifndef TABLE_H
#define TABLE_H

/*
typedef struct item
{
	char* data;
	struct keyspace* ks;
}item;*/

typedef struct node
{
	int rel;
	int offset;
	int len;
	//item* item;
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

//methods
int input(char* fn, Table* t);

typedef enum ERR
{
	ERR_OK,
	ERR_FILE,
	ERR_FREAD
}ERR;

#endif
