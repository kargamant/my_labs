#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hash.h"
#include "../Graph.h"

int New(int msize, Table* ht)
{
	//Table* ht=(Table*)malloc(sizeof(Table));
	ht->msize=msize;
	ht->ks=(KeySpace*)malloc(msize*sizeof(KeySpace));
	for(KeySpace* ptr=ht->ks; ptr-ht->ks<ht->msize; ++ptr)
	{
		ptr->busy=FREE;
		ptr->key="\0";
		ptr->info=(Item*)malloc(sizeof(Item));
		ptr->info->vertex=VertexInit(ENTER);
	}
	return ERR_OK;
}

Table* create()
{
	return (Table*)malloc(sizeof(Table));
}

void erased(Table* t)
{
	if(t->ks)
	{
		for(KeySpace* ptr=t->ks; ptr-t->ks<t->msize; ++ptr)
		{
			//if(ptr->busy==DELETED) free(ptr->info);
			if(ptr->busy==BUSY) free(ptr->key);
			Edge* pr=ptr->info->vertex->head;
			while(pr)
			{
				Edge* next=pr->next;
				//free(pr->to);
				free(pr);
				pr=NULL;
				pr=next;
			}
			free(ptr->info->vertex);
			free(ptr->info);
			//free(ptr->info);
		}
		free(t->ks);
	}
	free(t);
}


int add(Table* t, char* key, Room type)
{
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy!=BUSY) 
		{
			t->ks[j].busy=BUSY;
			t->ks[j].key=key;
			t->ks[j].info->vertex->type=type;
			return j;
		}
	}
	return -1;
}

int Del(Table* t, char* key)
{
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i,t->msize);
		if(t->ks[j].busy==FREE) break;
		if(t->ks[j].busy==BUSY && !strcmp(t->ks[j].key, key)) 
		{
			t->ks[j].busy=DELETED;
			free(t->ks[j].key);
			t->ks[j].key=NULL;
			Edge* ptr=t->ks[j].info->vertex->head;
			while(ptr)
			{
				Edge* next=ptr->next;
				//free(ptr->to);
				free(ptr);
				ptr=NULL;
				ptr=next;
			}
			return j;
		}
	}
	return -1;
}

int Search(Table* t, char* id)
{
	for(int i=0; i<t->msize; i++)
	{
		int j=h(id, i, t->msize);
		if(t->ks[j].busy==BUSY && !strcmp(t->ks[j].key, id)) return j;
		if(t->ks[j].busy==FREE) return -1;
	}
	return -1;
}

/*
void output(Table* t)
{
	printf("msize: %d\n", t->msize);
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->msize)
	{
		if(ptr->busy==BUSY) 
		{
			//if(t->msize==1) outputks(ptr);
			int h1d=h1(ptr->key, t->msize);
			int h2d=h2(ptr->key, t->msize);
			printf("h1: %d | h2: %d | ", h1d, h2d);
			printf("index: %d |", ptr-t->ks);
			outputks(ptr);
		}
		++ptr;
		//printf("db1\n");
	}
	printf("\n");
}

void outputks(KeySpace* ptr)
{
	printf("%d | %d | %s\n", ptr->key, ptr->rel, ptr->info->data);
}*/
