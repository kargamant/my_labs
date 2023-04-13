#ifndef HASHB_H
#define HASHB_H
#include <stdio.h>

typedef struct KeySpace
{
	int busy;//0 - free for insertion, 1 - busy, -1 - deleted
	unsigned int key;
	int rel;
	int offset;
	int len;
}KeySpace;

typedef struct Table
{
	int msize;
	FILE* fd;
	int i; //last research number
	KeySpace* ks;
}Table;

//methods
Table* create();
int New(int msize, Table* ht);
int input(char* fn, Table* t);
int TableWrite(Table* t);
Table* SearchByVersion(Table* t, int key, int rel);
Table* SearchByKey(Table* t, int key);
int add(Table* t, int key, char* data);
int DelByKey(Table* t, int key);
int DelByVersion(Table* t, int key, int rel);
void output(Table* t);
void outputks(KeySpace* ptr);
void erased(Table* t);


typedef enum STATES
{
	BUSY=1,
	FREE=0,
	DELETED=-1
}STATES;

typedef enum ERR
{
	ERR_OK,
	ERR_NF,
	ERR_FULL,
	ERR_FREAD,
	ERR_FWRITE
}ERR;


#endif
