#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Table.h"

int input(char* fn, Table* t)
{
	FILE* fd=fopen(fn, "r+b");
	if(!fd) return ERR_FILE;
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

void outputnd(Node* gr)
{
	printf("{%d, %s} ", gr->rel, gr->item->data);
}
