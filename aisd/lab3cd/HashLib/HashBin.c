#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashBin.h"
#include "Hash.h"

//import from file
int input(char* fn, Table* t)
{
	fseek(t->fd, 0, SEEK_SET);
	if(!fread(&t->msize, sizeof(int), 1, t->fd)) return ERR_FREAD;
	t->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace));
	if(!fread(t->ks, sizeof(KeySpace), t->msize, t->fd)) return ERR_FREAD;
	return ERR_OK;
}

//Saving to file
int TableWrite(Table* t)
{
	fseek(t->fd, sizeof(int), SEEK_SET);
	if(!fwrite(t->ks, sizeof(KeySpace), t->msize, t->fd)) return ERR_FWRITE;
	return ERR_OK;
}

//Initializing table with 0's
int New(int msize, Table* ht)
{
	ht->msize=msize;
	ht->ks=(KeySpace*)malloc(msize*sizeof(KeySpace));
	fwrite(&ht->msize, sizeof(int), 1, ht->fd);
	for(KeySpace* ptr=ht->ks; ptr-ht->ks<ht->msize; ++ptr)
	{
		ptr->busy=FREE;
		ptr->key=0;
		ptr->rel=0;
		ptr->offset=0;
		ptr->len=0;
	}
	fwrite(ht->ks, sizeof(KeySpace), ht->msize, ht->fd);
	return ERR_OK;
}

Table* create()
{
	return (Table*)calloc(3, sizeof(Table));
}

//erasing
void erased(Table* t)
{
	if(t->ks)
	{
		free(t->ks);
	}
}

Table* SearchByVersion(Table* t, int key, int rel)
{
	//creating new table that'll be returned at the end
	Table* rt=create();
	rt->fd=t->fd;
	rt->msize=t->msize;
	rt->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace));
	for(KeySpace* ptr=rt->ks; ptr-rt->ks<rt->msize; ++ptr)
	{
		ptr->busy=FREE;
		ptr->key=0;
		ptr->rel=0;
		ptr->offset=0;
		ptr->len=0;
	}

	//iterating
	for(int i=0; i<t->msize; i++)
	{
		int j=h(key, i, t->msize);

		//if we face free position than there will be no positions with that key further
		if(t->ks[j].busy==FREE) 
		{
			erased(rt);
			free(rt);
			return NULL;
		}
		//Successfully found
		if(t->ks[j].busy==BUSY && t->ks[j].key==key && t->ks[j].rel==rel) 
		{ 
			rt->ks[j].busy=BUSY;
			rt->ks[j].key=t->ks[j].key;
			rt->ks[j].rel=t->ks[j].rel;
			rt->ks[j].offset=t->ks[j].offset;
			rt->ks[j].len=t->ks[j].len;
			return rt;
		}
	}
	erased(rt);
	free(rt);
	return NULL;
}

//Similar to SearchByVersion, but it finds all releases of a key
Table* SearchByKey(Table* t, int key)
{
	Table* rt=create();
	rt->fd=t->fd;
	
	rt->msize=t->msize;
	rt->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace));
	for(KeySpace* ptr=rt->ks; ptr-rt->ks<rt->msize; ++ptr)
	{
		ptr->busy=FREE;
		ptr->key=0;
		ptr->rel=0;
		ptr->offset=0;
		ptr->len=0;
	}
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
			rt->ks[j].busy=BUSY;
			rt->ks[j].key=t->ks[j].key;
			rt->ks[j].rel=t->ks[j].rel;
			rt->ks[j].offset=t->ks[j].offset;
			rt->ks[j].len=t->ks[j].len;
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

//Adding to table
int add(Table* t, int key, char* data)
{	
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

	//Writing in the end of file
	fseek(t->fd, 0, SEEK_END);
	t->ks[fpos].offset=ftell(t->fd);
	t->ks[fpos].len=strlen(data)+1;
	fwrite(data, sizeof(char), t->ks[fpos].len, t->fd);
	return ERR_OK;
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
			//free(t->ks[j].info->data);
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
			//free(t->ks[j].info->data);
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
			int h1d=h1(ptr->key, t->msize);
			int h2d=h2(ptr->key, t->msize);
			printf("h1: %d | h2: %d | ", h1d, h2d);
			printf("index: %d |", ptr-t->ks);
			char* data=(char*)malloc(ptr->len*sizeof(char));
			fseek(t->fd, ptr->offset, SEEK_SET);
			fread(data, sizeof(char), ptr->len, t->fd);
			printf("%d | %d | %s\n", ptr->key, ptr->rel, data);
			free(data);
		}
		++ptr;
		//printf("db1\n");
	}
	printf("\n");
}

