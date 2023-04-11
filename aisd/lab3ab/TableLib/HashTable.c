#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "Hash.h"

int New(int msize, Table* ht)
{
	//Table* ht=(Table*)malloc(sizeof(Table));
	ht->msize=msize;
	ht->ks=(KeySpace*)malloc(msize*sizeof(KeySpace));
	for(KeySpace* ptr=ht->ks; ptr-ht->ks<ht->msize; ++ptr)
	{
		ptr->busy=FREE;
		ptr->key=0;
		ptr->rel=0;
		ptr->info=(Item*)malloc(sizeof(Item));
		ptr->info->data="\0";
	}
	return ERR_OK;
}

Table* create()
{
	return (Table*)calloc(2, sizeof(Table));
}

void erased(Table* t)
{
	for(KeySpace* ptr=t->ks; ptr-t->ks<t->msize; ++ptr)
	{
		free(ptr->info->data);
		free(ptr->info);
	}
	free(t->ks);
	free(t);
}

Table* SearchByVersion(Table* t, int key, int rel)
{
	Table* rt=create();
	New(1, rt);
	/*Table* rt=(Table*)malloc(sizeof(Table));
	rt->msize=1;
	rt->ks=(KeySpace*)malloc(rt->msize*sizeof(KeySpace));*/
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy==FREE) 
		{
			/*free(rt->ks);
			rt->ks=NULL;
			free(rt);
			rt=NULL;*/
			return NULL;
		}
		if(t->ks[j].busy==BUSY && t->ks[j].key==key && t->ks[j].rel==rel) 
		{
			rt->ks->busy=1;
			rt->ks->key=t->ks[j].key;
			rt->ks->rel=t->ks[j].rel;
			rt->ks->info=t->ks[j].info;
			return rt;
		}
	}
	/*free(rt->ks);
	rt->ks=NULL;
	free(rt);
	rt=NULL;*/
	return NULL;
}

Table* SearchByKey(Table* t, int key)
{
	Table* rt=create();
	New(t->msize, rt);
	/*Table* rt=(Table*)malloc(sizeof(Table));
	rt->msize=t->msize;
	rt->ks=(KeySpace*)calloc(rt->msize, rt->msize*sizeof(KeySpace));*/
	int n=0;
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy==FREE) 
		{
			break;
		}
		if(t->ks[j].busy==BUSY && t->ks[j].key==key)
		{
			add(rt, t->ks[j].key, t->ks[j].info->data);
			n++;
		}
	}
	if(!n) return NULL;
	return rt;
}

int add(Table* t, int key, char* data)
{
	//checking for last release
	/*Table* checkt=SearchByKey(t, key);
	int mrel=0;
	for(KeySpace* ks=checkt->ks; ks-checkt->ks<checkt->msize; ++ks)
	{
		if(ks->rel>mrel) mrel=ks->rel;
	}
	free(checkt->ks);
	checkt->ks=NULL;
	free(checkt);
	checkt=NULL;*/
	int mrel=0;
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy==FREE) break;
		if(t->ks[j].busy==BUSY && t->ks[j].key==key && t->ks[j].rel>mrel) mrel=t->ks[j].rel; 
	}
	//Remember free position and continue finding maxrel. In the end just insert to remembeered position maxrel
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy!=BUSY)
		{
			t->ks[j].busy=BUSY;
			t->ks[j].key=key;
			t->ks[j].rel=mrel+1;
			t->ks[j].info->data=data;
			return ERR_OK;
		}
	}
	return ERR_FULL;
}

int DelByKey(Table* t, int key)
{
	int n=0;
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i,t->msize);
		if(t->ks[j].busy==FREE) break;
		if(t->ks[j].busy==BUSY && t->ks[j].key==key) t->ks[j].busy=DELETED;
		n++;
	}
	if(n==0) return ERR_NF;
	return ERR_OK;
}

int DelByVersion(Table* t, int key, int rel)
{
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy==FREE) return ERR_NF;
		if(t->ks[j].busy==BUSY && t->ks[j].key==key, t->ks[j].rel==rel) 
		{
			t->ks[j].busy=DELETED;
			return ERR_OK;
		}
	}
}

void output(Table* t)
{
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->msize)
	{
		if(ptr->busy==1) 
		{
			if(t->msize==1) outputks(ptr);
			else
			{
				int h1d=h1(ptr->key, t->msize);
				int h2d=h2(ptr->key, t->msize);
				printf("h1: %d | h2: %d | ", h1d, h2d);
				printf("index: %d |", ptr-t->ks);
				outputks(ptr);
			}
		}
		++ptr;
		//printf("db1\n");
	}
	printf("\n");
}

void outputks(KeySpace* ptr)
{
	printf("%d | %d | %s\n", ptr->key, ptr->rel, ptr->info->data);
}
