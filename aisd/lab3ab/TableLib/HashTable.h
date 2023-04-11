#ifndef HASHT_H
#define HASHT_H

typedef struct Item
{
	char* data;
}Item;

typedef struct KeySpace
{
	int busy; //0 - free for insertion, 1 - busy, -1 - deleted
	unsigned int key;
	int rel;
	Item* info;
}KeySpace;

typedef struct Table
{
	int msize;
	KeySpace* ks;
}Table;

typedef enum STATES
{
	BUSY=1,
	FREE=0,
	DELETED=-1
}STATES;

//methods
Table* create();
int New(int msize, Table* ht);
Table* SearchByVersion(Table* t, int key, int rel);
Table* SearchByKey(Table* t, int key);
int add(Table* t, int key, char* data);
int DelByKey(Table* t, int key);
int DelByVersion(Table* t, int key, int rel);
void output(Table* t);
void outputks(KeySpace* ptr);
void erased(Table* t);

typedef enum ERR
{
	ERR_OK,
	ERR_NF,
	ERR_FULL

}ERR;

#endif
