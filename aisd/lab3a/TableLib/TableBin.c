#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TableBin.h"
#include "Table.h"

Table* create()
{
	return (Table*)calloc(4, sizeof(Table));
}

int New(int msize, Table* t)
{
	t->msize=msize;
	t->ks=(KeySpace*)malloc(msize*sizeof(KeySpace));
	FILE* fd=fopen(t->fd, "w+b");
	if(!fd) return ERR_FIL;
	if(!fwrite(&(t->msize), sizeof(int), 1, fd)) return ERR_FWRITE;
	fclose(fd);
	return ERR_OK;
}

int input(char* fn, Table* t)
{
	FILE* fd=fopen(fn, "r+b");
	if(!fd) return ERR_FIL;
	if(!fread(&(t->msize), sizeof(int), 1, fd)) 
	{
		fclose(fd);
		return ERR_FREAD;
	}
	t->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace));
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->msize && fread(&(ptr->key), sizeof(int), 1, fd))
	{
		int m=0;
		if(!fread(&m, sizeof(int), 1, fd)) 
		{
			fclose(fd);
			return ERR_FREAD;
		}
		ptr->Node=(Node*)malloc(sizeof(Node));
		Node* gr=ptr->Node;
		while(m)
		{
			if(!fread(&(gr->rel),sizeof(int), 1, fd) || !fread(&(gr->offset), sizeof(int), 1, fd) || !fread(&(gr->len), sizeof(int), 1, fd))
			{
				fclose(fd);
				return ERR_FREAD;
			}
			m--;
			if(!m) break;
			gr->next=(Node*)malloc(sizeof(Node));
			gr=gr->next;
		}
		//free(gr);
		//gr=NULL;
		++ptr;
		t->csize+=1;
	}
	fclose(fd);
	return ERR_OK;
}

int TableWrite(Table* t, char* fn)
{
	FILE* fd=fopen(fn, "w+b");
	if(!fd) return ERR_FIL;
	rewind(fd);
	if(!fwrite(&(t->msize), sizeof(int), 1, fd)) 
	{
		fclose(fd);
		return ERR_FWRITE;
	}
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize && fwrite(&(ptr->key), sizeof(int), 1, fd))
	{
		Node* gr=ptr->Node;
		int m=0;
		while(gr)
		{
			m++;
			gr=gr->next;
		}
		gr=ptr->Node;
		if(!fwrite(&m, sizeof(int), 1, fd)) 
		{
			fclose(fd);
			return ERR_FWRITE;
		}
		while(gr)
		{
			if(!fwrite(&(gr->rel), sizeof(int), 1, fd) || !fwrite(&(gr->offset), sizeof(int), 1, fd) || !fwrite(&(gr->len), sizeof(int), 1, fd))
			{
				fclose(fd);
				return ERR_FWRITE;
			}
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
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize)
	{
		outputksf(ptr, t->fi);
		++ptr;
	}
	//for(KeySpace* ptr=t->ks; ptr-t->ks<t->csize; ++ptr) outputksf(ptr, t->fi);
}

void outputksf(KeySpace* ptr, char* fn)
{
	printf("key %d | values: ", ptr->key);
	Node* gr=ptr->Node;
	while(gr)
	{
		outputndf(gr, fn);
		gr=gr->next;
	}
	printf("\n");
}

void outputndf(Node* gr, char* fn)
{
	FILE* fd=fopen(fn, "r+b");
	//char* data=(char*)malloc(gr->len*sizeof(char));
	fseek(fd, gr->offset, SEEK_SET);
	char* data=(char*)malloc(gr->len*sizeof(char));
	fread(data, sizeof(char), gr->len, fd);
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
		Node* gr=ptr->Node;
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
	free(t->fi);
	free(t->fd);
	t->fi=NULL;
	t->fd=NULL;
}

KeySpace* SearchByKey(Table* t, int key)
{
	KeySpace* ptr=t->ks;
	if(!ptr) return NULL;
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
	if(!ptr) return NULL;
	int f=0;
	while(ptr-t->ks<t->csize)
	{
		if(ptr->key==key)
		{
			Node* gr=ptr->Node;
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
	Node* gr=ks->Node;
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
	if(!ks->Node->next && ks->Node->rel==rel)
	{
		DelByKey(t, key);
		return ERR_OK;
	}
	else if(!ks->Node->next) return ERR_NO_FOUND;
	Node* gr=ks->Node;
	Node* prev=ks->Node;
	Node* next=NULL;
	while(gr)
	{
		if(gr->rel==rel)
		{
			//free(gr->item->data);
			//free(gr->item);
			next=gr->next;
			if(prev!=gr) prev->next=next;
			else ks->Node=next;
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
		newks->Node=(Node*)malloc(sizeof(Node));
		newks->Node->rel=1;
		FILE* fd=fopen(t->fi, "a");
		newks->Node->offset=ftell(fd);
		printf("offset: %d\n", newks->Node->offset);
		newks->Node->len=strlen(c)+1;
		fwrite(c, sizeof(char), newks->Node->len, fd);
		fclose(fd);
		newks->Node->next=NULL;
		t->csize+=1;
	}
	else
	{
		Node* gr=(Node*)malloc(sizeof(Node));
		Node* second=ks->Node;
		gr->rel=second->rel+1;
		FILE* fd=fopen(t->fi, "a");
		//fseek(fd, 0, SEEK_END);
		gr->offset=ftell(fd);
		printf("offset: %d\n", gr->offset);
		gr->len=strlen(c)+1;
		fseek(fd, gr->offset, SEEK_SET);
		fwrite(c, sizeof(char), gr->len, fd);
		fclose(fd);
		gr->next=second;
		ks->Node=gr;	
	}
	return ERR_OK;
}
