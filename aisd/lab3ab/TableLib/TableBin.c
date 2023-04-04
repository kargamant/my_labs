#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TableBin.h"
#include "Table.h"

Table* create()
{
	return (Table*)calloc(5, sizeof(Table));
}

int New(int msize, Table* t)
{
	FILE* fd=fopen(t->fd, "w+b");
	//if(!fd) fd=fopen(t->fd, "w+b");
	t->msize=msize;
	t->ks=(KeySpace*)malloc(msize*sizeof(KeySpace));
	if(!fd) return ERR_FIL;
	if(!fwrite(&(t->msize), sizeof(int), 1, fd)) return ERR_FWRITE;
	fclose(fd);
	return ERR_OK;
}

int input(char* fn, Table* t)
{
	FILE* fd=fopen(t->fd, "r+b");
	if(!fd) return ERR_FIL;
	if(!fread(&(t->msize), sizeof(int), 1, fd)) 
	{
		fclose(fd);
		return ERR_FREAD;
	}
	t->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace));
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->msize)
	{
		fread(&(ptr->key), sizeof(int), 1, fd);
		if(feof(fd)) break;
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
			if(!m) 
			{
				gr->next=NULL;
				break;
			}
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
	if(!t->fd) return ERR_FWRITE;
	FILE* fd=fopen(t->fd, "w+b");
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
}

void outputksf(KeySpace* ptr, FILE* fn)
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

void outputndf(Node* gr, FILE* fn)
{
	FILE* fd=fn;
	fseek(fd, gr->offset, SEEK_SET);
	char* data=(char*)malloc(gr->len*sizeof(char));
	fread(data, sizeof(char), gr->len, fd);
	printf("{%d, %s} ", gr->rel, data);
	free(data);
	data=NULL;
	//fclose(fd);
}

void erased(Table* t)
{
	KeySpace* ptr=t->ks;
	if(!ptr) goto ending;
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
	t->msize=0;
	t->csize=0;
	free(t->ks);
	t->ks=NULL;
	goto ending;
	ending:
	free(t->fd);
	t->fd=NULL;
	if(t->fi)
	{
	
		fclose(t->fi);
		//fclose(t->fd);
		t->fi=NULL;
		//t->fd=NULL;
	}
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
	if(!ks)
	{
		if(t->csize==t->msize) return ERR_FULL;
		KeySpace* newks=&((t->ks)[t->csize]);
		newks->key=key;
		newks->Node=(Node*)malloc(sizeof(Node));
		newks->Node->rel=1;
		FILE* fd=t->fi;
		fseek(fd, 0, SEEK_END);
		newks->Node->offset=ftell(fd);
		newks->Node->len=strlen(c)+1;
		fwrite(c, sizeof(char), newks->Node->len, fd);
		newks->Node->next=NULL;
		t->csize+=1;
		/*fd=t->fd;
		fwrite(&(newks->key), sizeof(int), 1, fd);
		fwrite(&(newks->Node->rel), sizeof(int), 1, fd);
		fwrite(&(newks->Node->rel), sizeof(int), 1, fd);
		fwrite(&(newks->Node->offset), sizeof(int), 1, fd);
		fwrite(&(newks->Node->len), sizeof(int), 1, fd);*/
		//fclose(fd);
	}
	else
	{
		Node* gr=(Node*)malloc(sizeof(Node));
		Node* second=ks->Node;
		gr->rel=second->rel+1;
		gr->next=second;
		ks->Node=gr;

		//FILE* fd=fopen(t->fi, "a");
		FILE* fd=t->fi;
		fseek(fd, 0, SEEK_END);
		gr->offset=ftell(fd);
		gr->len=strlen(c)+1;
		fwrite(c, sizeof(char), gr->len, fd);
		//fclose(fd);
		/*fd=t->fd;	
		int rk=-1;
		while(rk!=key) fread(&rk, sizeof(int), 1, fd);
		int off=ftell(fd);
		int oldm;
		fread(&oldm, sizeof(int), 1, fd);
		fseek(fd, off, SEEK_SET);
		int newm=oldm+1;
		fwrite(&newm, sizeof(int), 1, fd);
		int iff=ftell(fd);
		//fclose(fd);
		//fclose(fd);
		//fd=fopen(t->fd, "a");
		fseek(fd, 0, SEEK_END);
		//fd=fopen(t->fd, "a+");
		int r=73;
		for(int i=0; i<3; i++) fwrite(&r, sizeof(int), 1, fd);
		int yff=ftell(fd);
		
		//printf("iff yff: %d %d\n", iff, yff);
		for(int i=1; i<=(yff-iff)/4 -3; i++)
		{
			int link;
			fseek(fd, -4*(3+i), SEEK_END);
			fread(&link, sizeof(int), 1, fd);
			fseek(fd, -4*i, SEEK_END);
			fwrite(&link, sizeof(int), 1, fd);
		}
		fseek(fd, iff-yff, SEEK_END);
		fwrite(&(gr->rel), sizeof(int), 1, fd);
		fwrite(&(gr->offset), sizeof(int), 1, fd);
		fwrite(&(gr->len), sizeof(int), 1, fd);*/
		//fclose(fd);
		/*
		fseek(fd, 0, SEEK_END);
		fseek(fd, -4*sizeof(int), SEEK_END);*/
	}
	return ERR_OK;
}
