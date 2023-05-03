#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"

Node* Min(Btree* tr)
{
	Node* ptr=tr->root;
	while(*(ptr->child)) ptr=*(ptr->child);
	return ptr;
}

int Traversing(Btree* tr, int key)
{
	Node* ptr=Min(tr);
	
	for(int i=0; i<ptr->n; i++)
	{
		if(ptr->keys[i]<=key)
		{
			printf("key: %d | ", ptr->keys[i]);
			printf("info: %s\n", ptr->info[i]);
		}
		else return ERR_OK;
	}
	ptr=ptr->par;
	for(int i=0; i<ptr->n; i++)
	{
		if(ptr->keys[i]<=key)
		{
			printf("key: %d | ", ptr->keys[i]);
			printf("info: %s\n", ptr->info[i]);
			Btree* ntr=(Btree*)malloc(sizeof(Btree));
			ntr->t=tr->t;
			ntr->root=ptr->child[i+1];
			Traversing(ntr, key);
			free(ntr);
		}
		else return ERR_OK;
	}
	return ERR_OK;
}

Item* Search(Btree* tr, int key)
{
	Item* list=NULL;
	Item* head=list;
	Node* ptr=tr->root;
	while(ptr)
	{
		int f=0;
		//make pointer iteration
		for(int i=0; i<ptr->n; i++)
		{
			if(ptr->keys[i]>key)
			{
				ptr=ptr->child[i];
				f=1;
				break;
			}
			else if(ptr->keys[i]==key)
			{
				list=(Item*)malloc(sizeof(Item));
				list->node=ptr;
				list->next=NULL;
				list=list->next;
				//Because adding of the duplicate key will be at the first available place
				if(!head->next)
				{
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=*(ptr->child);
					Item* same=Search(ntr, key);
					free(ntr);	
					list=same;
				}
				f=1;
			}
		}
		if(!f) ptr=ptr->child[ptr->n];
	}
	return head;
}

int AddNode(Btree* tr, int key, char* info)
{
	Node* ptr=tr->root;

	//searching for a leaf to insert
	while(ptr->child)
	{
		int f=0;
		for(int i=0; i<ptr->n; i++)
		{
			if(ptr->keys[i]>=key)
			{
				ptr=ptr->child[i];
				f=1;
				break;
			}
		}
		if(!f) ptr=ptr->child[ptr->n];
	}
	if(ptr->n==2*tr->t)
	{
		//Need splitting function
		//...
	}
	else
	{
		for(int i=ptr->n-1; i>=0; i--)
		{
			if(ptr->keys[i]>key)
			{
				ptr->keys[2*tr->t-(ptr->n-i+1)]=ptr->keys[i];
				ptr->info[2*tr->t-(ptr->n-i+1)]=ptr->info[i];
			}
			else
			{
				ptr->keys[i+1]=key;
				ptr->info[i+1]=info;
			}
		}
	}
	ptr->n=ptr->n+1;
	return ERR_OK;
}

void Split(Btree* tr, Node* x, int i)
{	
	Node* ptr=x->child[i];
	Node* z=(Node*)malloc(sizeof(Node));
	z->n=0;
	z->keys=(int*)malloc((2*tr->t -1)sizeof(int));
	z->info=(char**)malloc((2*tr->t -1)sizeof(char*));
	z->child=(Node**)malloc(2*tr->t*sizeof(Node*));
	z->par=x;

	//will be remade with pointer iteration
	for(int j=x->n+1; j>i+1; j--)
	{
		x->child[j]=x->child[j-1];
	}
	x->child[i+1]=z;
	for(int j=1+(2*tr->t -1)/2; j<ptr->n; j++)
	{
		z->keys[j-(1+(2*tr->t -1)/2)]=ptr->keys[1+(2*tr->t -1)/2];
		z->info[j-(1+(2*tr->t -1)/2)]=ptr->info[1+(2*tr->t -1)/2];
		z->n=z->n+1;
	}
	for(int j=ptr->n-1; j>i; j--)
	{
		
	}
}























