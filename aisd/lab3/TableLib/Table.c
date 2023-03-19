#include "Table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../funcs.h"

//parsing table from text file
int input(FILE* fd, Table** t)
{
	//Table* t=(Table*)malloc(sizeof(Table));
	Table* y=*t;
	y->msize=GetIntf(fd);
	if(y->msize==-1) return ERR_WRD;
	y->csize=0;
	y->ks=(KeySpace*)malloc((y->msize)*sizeof(KeySpace));
	KeySpace* ptr=y->ks;
	while(ptr-y->ks<y->msize && (ptr->key=GetIntf(fd))!=-1)
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
		int k=1;
		ptr->node=(Node*)malloc(sizeof(Node));
		Node* cur=ptr->node;
		Node* prev=ptr->node;
		while(tok)
		{
			cur->item=(Item*)malloc(sizeof(Item));
			cur->item->data=strdup(tok);
			cur->item->ks=ptr;
			prev->next=cur;
			prev=cur;
			tok=strtok(NULL, " ");
			if(tok) cur->next=(Node*)malloc(sizeof(Node));
			else cur->next=NULL;
			cur=cur->next;
			k++;
		}
		cur=ptr->node;
		k--;
		while(k && cur)
		{
			cur->rel=k;
			k--;
			cur=cur->next;
		}
		free(s);
		s=NULL;
		y->csize+=1;
		++ptr;
	}
	return ERR_OK;
}
/*
int readt(FILE* fd, Table* t)
{
	int nmsize=GetIntf(fd);
	t->ks=(KeySpace*)realloc(t->ks, nmsize*sizeof(KeySpace));
	t->msize=nmsize;
	int ncize=0;
	KeySpace* ptr=t->ks;
	//else
	//{
	//	for(KeySpace* ptr=t->ks + nmsize; ptr-t->ks<t->csize
	//}
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
		int k=1;
		//ptr->node=(Node*)malloc(sizeof(Node));
		Node* cur=ptr->node;
		Node* prev=ptr->node;
		while(tok)
		{
			//cur->item=(Item*)malloc(sizeof(Item));
			cur->item->data=strdup(tok);
			cur->item->ks=ptr;
			prev->next=cur;
			prev=cur;
			tok=strtok(NULL, " ");
			if(tok) cur=cur->next;
			else cur->next=NULL;
			cur=cur->next;
			k++;
		}
		cur=ptr->node;
		k--;
		while(k && cur)
		{
			cur->rel=k;
			k--;
			cur=cur->next;
		}
		free(s);
		s=NULL;
		t->csize+=1;
		++ptr;
	}
	return ERR_OK;

}*/

//full clearing of table
void erased(Table* t)
{
	KeySpace* ptr=t->ks;
	if(ptr) 
	{
		while(ptr-t->ks<t->csize)
		{
			Node* gr=ptr->node;
			if(!gr) continue;
			while(gr)
			{
				free(gr->item->data);
				gr->item->data=NULL;
				free(gr->item);
				gr->item=NULL;
				Node* next=gr->next;
				free(gr);
				gr=next;
			}
			++ptr;
		}
		free(t->ks);
		t->ks=NULL;
	}
	
	//free(t);
	//t=NULL;
}

//diffrent outputs. for table, keyspace and node
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

//search all elements by key
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

//search one specific element by key and it's version
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

//adding new element
int add(Table* t, int key, char* c)
{	
	KeySpace* ks=SearchByKey(t, key);
	char* nc=strdup(c);
	if(!ks)
	{
		if(t->csize==t->msize) return ERR_FULL;
		KeySpace* newks=&((t->ks)[t->csize]);
		newks->key=key;
		newks->node=(Node*)malloc(sizeof(Node));
		newks->node->rel=1;
		newks->node->item=(Item*)malloc(sizeof(Item));
		newks->node->item->data=nc;
		newks->node->item->ks=newks;
		newks->node->next=NULL;
		t->csize+=1;
	}
	else
	{
		Node* gr=(Node*)malloc(sizeof(Node));
		Node* second=ks->node;
		gr->rel=second->rel+1;
		gr->item=(Item*)malloc(sizeof(Item));
		gr->item->data=nc;
		gr->item->ks=ks;
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

//deleting all elements by key
int DelByKey(Table* t, int key)
{
	KeySpace* ks=SearchByKey(t, key);
	if(!ks) return ERR_NO_FOUND;
	Node* gr=ks->node;
	while(gr)
	{
		free(gr->item->data);
		free(gr->item);
		Node* next=gr->next;
		free(gr);
		gr=next;
	}
	*ks=*(t->ks+t->csize-1);	
	t->csize-=1;
	return ERR_OK;
}

//deleting one specific element by key and it's version
int DelByVersion(Table* t, int key, int rel)
{
	KeySpace* ks=SearchByKey(t, key);
	if(!ks) return ERR_NO_FOUND;
	if(rel==1 && !ks->node->next)
	{
		DelByKey(t, key);
		return ERR_OK;
	}
	Node* gr=ks->node;
	Node* prev=ks->node;
	Node* next=NULL;
	while(gr)
	{
		if(gr->rel==rel)
		{
			free(gr->item->data);
			free(gr->item);
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
