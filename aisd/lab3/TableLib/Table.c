#include "Table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../funcs.h"

Table* input(FILE* fd)
{
	Table* t=(Table*)malloc(sizeof(Table));
	int msize=0;
	fscanf(fd, "%d", &msize);
	t->msize=msize;
	t->csize=0;
	t->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace));
	fscanf(fd, "%c");
	KeySpace* ptr=t->ks;
	printf("debug1\n");
	while(fscanf(fd, "%d", ptr->key))
	{
		fscanf(fd, "%c");
		char* s=enterf(fd);
		char* tok=strtok(s, " ");
		int k=0;
		ptr->node=(Node*)malloc(sizeof(Node));
		Node* cur=ptr->node;
		Node* prev=ptr->node;
		while(tok)
		{
			cur->rel=k;
			cur->item->data=tok;
			cur->item->ks=ptr;
			prev->next=cur;
			prev=cur;
			cur->next=(Node*)malloc(sizeof(Node));
			cur=cur->next;
			tok=strtok(NULL, " ");
			k++;
		}
		++ptr;
	}
	return t;
}
