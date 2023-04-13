#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	if(t->ks)
	{
		for(KeySpace* ptr=t->ks; ptr-t->ks<t->msize; ++ptr)
		{
			//if(ptr->busy==DELETED) free(ptr->info);
			if(ptr->busy==BUSY) free(ptr->info->data);
			free(ptr->info);
		}
		free(t->ks);
	}
	//free(t);
}

Table* SearchByVersion(Table* t, int key, int rel)
{
	Table* rt=create();
	New(t->msize, rt);
	/*Table* rt=(Table*)malloc(sizeof(Table));
	rt->msize=1;
	rt->ks=(KeySpace*)malloc(rt->msize*sizeof(KeySpace));*/
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy==FREE) 
		{
			erased(rt);
			free(rt);
			/*free(rt->ks);
			rt->ks=NULL;
			free(rt);
			rt=NULL;*/
			return NULL;
		}
		if(t->ks[j].busy==BUSY && t->ks[j].key==key && t->ks[j].rel==rel) 
		{ 
			rt->ks[j].busy=BUSY;
			rt->ks[j].key=t->ks[j].key;
			rt->ks[j].rel=t->ks[j].rel;
			//rt->ks->info=(Item*)malloc(sizeof(Item));
			//rt->ks->info=(t->ks+j)->info;
			rt->ks[j].info->data=strdup(t->ks[j].info->data);
			//rt->msize=t->msize;
			return rt;
		}
	}
	/*free(rt->ks);
	rt->ks=NULL;
	free(rt);
	rt=NULL;*/
	erased(rt);
	free(rt);
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
		//if(t->ks[j].busy==BUSY && t->ks[j].key!=key) rt->ks[j].busy=BUSY;
		if(t->ks[j].busy==BUSY && t->ks[j].key==key)
		{
			rt->ks[j].busy=BUSY;
			rt->ks[j].key=t->ks[j].key;
			rt->ks[j].rel=t->ks[j].rel;
			//rt->ks->info=(Item*)malloc(sizeof(Item));
			//rt->ks->info=(t->ks+j)->info;
			rt->ks[j].info->data=strdup(t->ks[j].info->data);
			//add(rt, t->ks[j].key, t->ks[j].info->data);
			n++;
		}
	}
	if(!n) 
	{
		erased(rt);
		free(rt);
		return NULL;
	}
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
	int fpos=-1, ind=t->msize;
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);
		if(t->ks[j].busy!=BUSY && i<ind) 
		{
			ind=i;
			fpos=j;
		}
		if(t->ks[j].busy==FREE) break;
		if(t->ks[j].busy==BUSY && t->ks[j].key==key && t->ks[j].rel>mrel) mrel=t->ks[j].rel; 
	}
	if(fpos==-1) return ERR_FULL;
	t->ks[fpos].busy=BUSY;
	t->ks[fpos].key=key;
	t->ks[fpos].rel=mrel+1;
	t->ks[fpos].info->data=strdup(data);
	return ERR_OK;
	//Remember free position and continue finding maxrel. In the end just insert to remembeered position maxrel
	/*for(int i=0; i<t->msize; i++)
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
	return ERR_FULL;*/
}

int DelByKey(Table* t, int key)
{
	int n=0;
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i,t->msize);
		if(t->ks[j].busy==FREE) break;
		if(t->ks[j].busy==BUSY && t->ks[j].key==key) 
		{
			t->ks[j].busy=DELETED;
			if(t->ks[j].info->data) free(t->ks[j].info->data);
			n++;
		}
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
		if(t->ks[j].busy==BUSY && t->ks[j].key==key && t->ks[j].rel==rel) 
		{
			t->ks[j].busy=DELETED;
			free(t->ks[j].info->data);
			return ERR_OK;
		}
	}
	return ERR_NF;
}

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
}
