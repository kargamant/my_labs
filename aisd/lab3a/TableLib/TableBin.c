#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TableBin.h"
#include "Table.h"

Table* create()
{
	return (Table*)calloc(3, sizeof(Table));
}
int input(char* fn, Table* t)
{
	FILE* fd=fopen(fn, "r+b");
	if(!fd) return ERR_FIL;
	if(!fread(&(t->msize), sizeof(int), 1, fd)) return ERR_FREAD;
	t->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace));
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->msize && fread(&(ptr->key), sizeof(int), 1, fd))
	{
		int m=0;
		if(!fread(&m, sizeof(int), 1, fd)) return ERR_FREAD;
		ptr->node=(Node*)malloc(sizeof(Node));
		Node* gr=ptr->node;
		while(m)
		{
			if(!fread(&(gr->rel),sizeof(int), 1, fd)) return ERR_FREAD;
			if(!fread(&(gr->offset), sizeof(int), 1, fd)) return ERR_FREAD;
			if(!fread(&(gr->len), sizeof(int), 1, fd)) return ERR_FREAD;
			m--;
			gr->next=(Node*)malloc(sizeof(Node));
			gr=gr->next;
		}
		++ptr;
		t->csize+=1;
	}
	fclose(fd);
	return ERR_OK;
}

int TableWrite(Table* t, char* fn)
{
	FILE* fd=fopen(fn, "w+b");
	rewind(fd);
	if(!fwrite(&(t->msize), sizeof(int), 1, fd)) return ERR_FWRITE;
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize && fwrite(&(ptr->key), sizeof(int), 1, fd))
	{
		Node* gr=ptr->node;
		int m=0;
		while(gr)
		{
			m++;
			gr=gr->next;
		}
		gr=ptr->node;
		if(!fwrite(&m, sizeof(int), 1, fd)) return ERR_FWRITE;
		while(gr)
		{
			if(!fwrite(&(gr->rel), sizeof(int), 1, fd)) return ERR_FWRITE;
			if(!fwrite(&(gr->offset), sizeof(int), 1, fd)) return ERR_FWRITE;
			if(!fwrite(&(gr->len), sizeof(int), 1, fd)) return ERR_FWRITE;
			gr=gr->next;
		}
		++ptr;
	}
	fclose(fd);
	return ERR_OK;
}


//to be rewritten
void output(Table* t)
{
	printf("msize: %d\n", t->msize);
	printf("csize: %d\n", t->csize);
	for(KeySpace* ptr=t->ks; ptr-t->ks<t->csize; ++ptr) outputks(ptr);
}

void outputks(KeySpace* ptr)
{
	printf("key %d | values: ", ptr->key);
	Node* gr=ptr->node;
	while(gr)
	{
		outputnd(gr);
		gr=gr->next;
	}
	printf("\n");
}

void outputndf(Node* gr, char* fn)
{
	FILE* fd=fopen(fn, "r+b");
	char* data=(char*)malloc(gr->len*sizeof(char));
	fseek(fd, gr->offset, SEEK_SET);
	fread(&data, sizeof(char), gr->len, fd);
	printf("{%d, %s} ", gr->rel, data);
	free(data);
	data=NULL;
	fclose(fd);
}

void erased(Table* t)
{
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize)
	{
		Node* gr=ptr->node;
		while(gr)
		{
			Node* next=gr->next;
			free(gr);
			gr=next;
		}
		++ptr;
	}
	free(t->ks);
	t->ks=NULL;
}

KeySpace* SearchByKey(Table* t, int key)
{
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize)
	{
		if(ptr->key==key) return ptr;
		++ptr;
	}
	return NULL;
}

Node* SearchByVersion(Table* t, int key, int rel)
{
	KeySpace* ptr=t->ks;
	int f=0;
	while(ptr-t->ks<t->csize)
	{
		if(ptr->key==key)
		{
			Node* gr=ptr->node;
			while(gr)
			{
				if(gr->rel==rel) return gr;
				gr=gr->next;
			}
			return NULL;
		}
		++ptr;
	}
	return NULL;
}

int DelByKey(Table* t, int key)
{
	KeySpace* ks=SearchByKey(t, key);
	if(!ks) return ERR_NO_FOUND;
	Node* gr=ks->node;
	while(gr)
	{
		//free(gr->item->data);
		//free(gr->item);
		Node* next=gr->next;
		free(gr);
		gr=next;
	}
	*ks=*(t->ks+t->csize-1);	
	t->csize-=1;
	return ERR_OK;
}

int DelByVersion(Table* t, int key, int rel)
{
	KeySpace* ks=SearchByKey(t, key);
	if(!ks) return ERR_NO_FOUND;
	if(!ks->node->next && ks->node->rel==rel)
	{
		DelByKey(t, key);
		return ERR_OK;
	}
	else if(!ks->node->next) return ERR_NO_FOUND;
	Node* gr=ks->node;
	Node* prev=ks->node;
	Node* next=NULL;
	while(gr)
	{
		if(gr->rel==rel)
		{
			//free(gr->item->data);
			//free(gr->item);
			next=gr->next;
			if(prev!=gr) prev->next=next;
			else ks->node=next;
			free(gr);
			gr=NULL;
			return ERR_OK;
		}
		prev=gr;
		gr=gr->next;
	}
	if(!next) return ERR_NO_FOUND;
	/*
	while(next)
	{
		next->rel-=1;
		next=next->next;
	}
	return ERR_OK;*/
}

int addf(Table* t, int key, char* c, char* fn)
{	
	KeySpace* ks=SearchByKey(t, key);
	//char* nc=strdup(c);
	if(!ks)
	{
		if(t->csize==t->msize) return ERR_FULL;
		KeySpace* newks=&((t->ks)[t->csize]);
		newks->key=key;
		newks->node=(Node*)malloc(sizeof(Node));
		newks->node->rel=1;
		FILE* fd=fopen(fn, "w+b");
		newks->node->offset=ftell(fd);
		newks->node->len=strlen(c)+1;
		fwrite(c, sizeof(char), newks->node->len, fd);
		fclose(fd);
		newks->node->next=NULL;
		t->csize+=1;
	}
	else
	{
		Node* gr=(Node*)malloc(sizeof(Node));
		Node* second=ks->node;
		gr->rel=second->rel+1;
		FILE* fd=fopen(fn, "w+b");
		gr->offset=ftell(fd);
		gr->len=strlen(c)+1;
		fwrite(c, sizeof(char), gr->len, fd);
		fclose(fd);
		gr->next=second;
		ks->node=gr;
		/*
		Node* gr=ks->node;
		while(gr->next) gr=gr->next;
		int rel=gr->rel +1;
		gr->next=(Node*)malloc(sizeof(Node));
		gr->next->rel=rel;
		gr->next->item=(Item*)malloc(sizeof(Item));
		gr->next->item->data=nc;
		gr->next->item->ks=ks;
		gr->next->next=NULL;*/
	}
	return ERR_OK;
}
