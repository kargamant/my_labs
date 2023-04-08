#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "Hash.h"

Table* SearchByVersion(Table* t, int key, int rel)
{
	Table* rt=(Table*)malloc(sizeof(Table));
	rt->msize=1;
	rt->ks=(KeySpace*)malloc(rt->msize*sizeof(KeySpace));
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j]->busy==FREE) 
		{
			free(rt->ks);
			rt->ks=NULL;
			free(rt);
			rt=NULL;
			return NULL;
		}
		if(t->ks[j]->busy==BUSY && t->ks[j]->key==key && t->ks[j]->rel==rel) 
		{
			rt->ks->busy=1;
			rt->ks->key=t->ks[j]->key;
			rt->ks->rel=t->ks[j]->rel;
			rt->ks->info=t->ks[j]->info;
			return rt;
		}
	}
	free(rt->ks);
	rt->ks=NULL;
	free(rt);
	rt=NULL;
	return NULL;
}

Table* SearchByKey(Table* t, int key)
{
	Table* rt=(Table*)malloc(sizeof(Table));
	rt->msize=t->msize;
	rt->ks=(KeySpace*)calloc(rt->msize*sizeof(KeySpace));
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j]->busy==FREE) 
		{
			free(rt->ks);
			rt->ks=NULL;
			free(rt);
			rt=NULL;
			return NULL;
		}
		if(t->ks[j]->busy==BUSY && t->ks[j]->key==key)
		{
			Add(rt, t->ks[j]->key, t->ks[j]->info);
		}
	}
	return rt;
}

int Add(Table* t, int key, char* data)
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
		if(t->ks[j]->busy==FREE) break;
		if(t->ks[j]->busy==BUSY && t->ks[j]->key==key && t->ks[j]->rel>mrel) mrel=t->ks[j]->rel; 
	}
	//Remember free position and continue finding maxrel. In the end just insert to remembeered position maxrel
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j]->busy!=BUSY)
		{
			t->ks[j]->busy=BUSY;
			t->ks[j]->key=key;
			t->ks[j]->rel=mrel+1;
			t->ks[j]->info=data;
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
		if(t->ks[j]->busy==FREE) break;
		if(t->ks[j]->busy==BUSY && t->ks[j]->key==key) t->ks[j]->busy=DELETED;
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
		if(t-ks[j]->busy==FREE) return ERR_NF;
		if(t->ks[j]->busy==BUSY && t->ks[j]->key==key, t->ks[j]->rel==rel) 
		{
			t->ks[j]->busy=DELETED;
			return ERR_OK;
		}
	}
}

void output(Table* t)
{
	for(KeySpace* ptr=t->ks; ptr-t->ks<t->
}
