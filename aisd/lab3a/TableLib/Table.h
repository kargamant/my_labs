#ifndef TABLE_H
#define TABLE_H
#include <stdio.h>

//structures
typedef struct item Item;
typedef struct node Node;
typedef struct keyspace KeySpace;
typedef struct table Table;

//methods
Table* create();
int New(int msize, Table* t);
//int Newf(int msize, Table* t, char* fn);
int input(char* fn, Table* t);
int TableWrite(Table*t, char* fn);
void erased(Table* t);
void output(Table* t);
void outputks(KeySpace* ptr);
void outputksf(KeySpace* ptr, char* fn);
void outputnd(Node* gr);
void outputndf(Node* gr, char* fn);
int DelByKey(Table* t, int key);
int DelByVersion(Table* t, int key, int rel);
int add(Table* t, int key, char* c);
int addf(Table* t, int key, char* c, char* fn);
KeySpace* SearchByKey(Table* t, int key);
Node* SearchByVersion(Table* t, int key, int rel);

//error codes constants
typedef enum ERR
{
	ERR_OK,
	ERR_EMPTY,
	ERR_FULL,
	ERR_NO_FOUND,
	ERR_DUP,
	ERR_WRD,
	ERR_FIL,
	ERR_FREAD,
	ERR_FWRITE
}ERR;

#endif
