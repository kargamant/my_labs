#ifndef HASHT_H
#define HASHT_H
#include "Hash.h"

typedef struct Item
{
	Vertex* vertex;
}Item;

typedef enum State
{
	BUSY=1,
	FREE=0,
	DELETED=-1
}State;

typedef struct KeySpace
{
	State busy; //0 - free for insertion, 1 - busy, -1 - deleted
	char* key;
	Item* info;
}KeySpace;

typedef struct Table
{
	int msize;
	KeySpace* ks;
}Table;


//methods
Table* create();
int New(int msize, Table* ht);
int add(Table* t, char* key, Room type);
int Del(Table* t, char* key);
void erased(Table* t);
int Search(Table* t, char* id);


/*typedef enum ERR
{
	ERR_OK,
	ERR_NF,
	ERR_FULL

}ERR;*/

#endif
