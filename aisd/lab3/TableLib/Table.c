#include "Table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../funcs.h"

Table* input(FILE* fd)
{
	Table* t=(Table*)malloc(sizeof(Table));
	t->msize=GetIntf(fd);
	if(t->msize==-1) return NULL;
	//fscanf(fd, "%d", &(t->msize));
	t->csize=0;
	t->ks=(KeySpace*)malloc((t->msize)*sizeof(KeySpace));
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->msize && (ptr->key=GetIntf(fd))!=-1)
	{
		char* s=enterf(fd);
		char* tok=strtok(s, " ");
		if(!tok) 
		{
			printf("Error. No value for key: %d\n", ptr->key);
			free(s);
			s=NULL;
			break;
		}
		int k=0;
		ptr->node=(Node*)malloc(sizeof(Node));
		Node* cur=ptr->node;
		Node* prev=ptr->node;
		while(tok)
		{
			cur->rel=k;
			cur->item=(Item*)malloc(sizeof(Item));
			//cur->item->data=(char*)malloc(sizeof(char)*strlen(tok));
			//for(int i=0; i<strlen(tok); i++) (cur->item->data)[i]=tok[i];
			cur->item->data=strdup(tok);
			//printf("data: %s\n", cur->item->data);
			cur->item->ks=ptr;
			prev->next=cur;
			prev=cur;
			tok=strtok(NULL, " ");
			if(tok) cur->next=(Node*)malloc(sizeof(Node));
			else cur->next=NULL;
			cur=cur->next;
			k++;
			//printf("data2: %s\n", cur->item->data);
		}
		//free(cur->next);
		free(s);
		s=NULL;
		t->csize+=1;
		++ptr;
	}
	return t;
}

void erase(Table* t)
{
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize)
	{
		Node* gr=ptr->node;
		while(gr)
		{
			free(gr->item->data);
			free(gr->item);
			Node* next=gr->next;
			free(gr);
			gr=next;
		}
		++ptr;
	}
	free(t->ks);
	free(t);
}

void output(Table* t)
{
	printf("msize: %d\n", t->msize);
	printf("csize: %d\n", t->csize);
	for(KeySpace* ptr=t->ks; ptr-t->ks<t->csize; ++ptr)
	{
		printf("key %d | values: ", ptr->key);
		Node* gr=ptr->node;
		while(gr)
		{
			printf("{%d, %s} ", gr->rel, gr->item->data);
			gr=gr->next;
		}
		printf("\n");
	}
}
