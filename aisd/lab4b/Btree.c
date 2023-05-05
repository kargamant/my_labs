#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"

Btree* InitBtree(int t)
{
	Btree* tr=(Btree*)malloc(sizeof(Btree));
	tr->t=t;
	tr->root=(Node*)malloc(sizeof(Node));
	Node* root=tr->root;
	root->n=0;
	root->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
	root->info=(Item*)calloc(2*tr->t -1, (2*tr->t -1)*sizeof(Item));
	root->child=(Node**)malloc((2*tr->t)*sizeof(Node*));
	for(Node** gr=root->child; gr-root->child<2*tr->t; ++gr) *gr=NULL;
	root->par=NULL;
	//for(int i=0; i<2*tr->t; i++) root->child[i]=NULL;
	return tr;	
}

/*
void erase(Btree* tr)
{
	Node* ptr=tr->root;
	if(ptr)
	{
		for(int i=0; i<ptr->n; i++)
		{
			Btree* ntr=(Btree*)malloc(sizeof(Btree));
			ntr->t=tr->t;
			ntr->root=ptr->child[i];
			erase(ntr);
			free(ntr);
			
		}
	}
}*/

Node* Min(Btree* tr)
{
	Node* ptr=tr->root;
	while(*(ptr->child)) ptr=*(ptr->child);
	return ptr;
}

int Traversing(Btree* tr, int key)
{
	Node* ptr=tr->root;
	if(!ptr) return 0;
	for(int i=0; i<ptr->n; i++)
	{
		//if(!ptr->info) continue;
		Btree* ntr=(Btree*)malloc(sizeof(Btree));
		ntr->t=tr->t;
		ntr->root=ptr->child[i];
		Traversing(ntr, key);
		free(ntr);
		if(ptr->keys[i]<=key)
		{
			printf("key: %d | info: ", ptr->keys[i]);
			Item* gr=ptr->info + i;
			while(gr)
			{
				printf("\"%s\", ", gr->data);
				gr=gr->next;
			}
			printf("\n");
			//printf("info: %s\n", ptr->info[i]);
		}
	}
	Btree* ntr=(Btree*)malloc(sizeof(Btree));
	ntr->t=tr->t;
	ntr->root=ptr->child[ptr->n];
	Traversing(ntr, key);
	free(ntr);
	return 0;
	/*
	//Traversing using parent
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
	while(ptr)
	{
		ptr=ptr->par;
		for(int i=0; i<ptr->n; i++)
		{
			if(ptr->keys[i]<=key)
			{
				printf("key: %d | ", ptr->keys[i]);
				printf("info: %s\n", ptr->info[i]);
				if(ptr->child[i+1])
				{
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=ptr->child[i+1];
					Traversing(ntr, key);
					free(ntr);
				}
			}
			else return ERR_OK;
		}
	}
	return ERR_OK;*/
}

Item* Search(Btree* tr, int key)
{
	//Item** result=(Item**)malloc(sizeof(Item*));
	int n=0;
	Node* ptr=tr->root;
	while(ptr)
	{
		int f=0;
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
				Item* gr=ptr->info + i;
				/*while(gr)
				{
					result[n]=gr;
					n++;
					if(gr->next) result=(Item**)realloc((n+1)*sizeof(Item*));
					gr=gr->next;
				}*/
				return gr;
			}
		}
		if(!f) ptr=ptr->child[ptr->n]; 
	}
	//free(result);
	return NULL;
}

int AddNode(Btree* tr, int key, char* info)
{
	Node* ptr=tr->root;
	Node* par=NULL;
	while(ptr)
	{
		int f=0;
		par=ptr;
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
				Item* old=(Item*)malloc(sizeof(Item));
				old->data=ptr->info[i].data;
				old->next=ptr->info[i].next;
				ptr->info[i].data=info;
				ptr->info[i].next=old;
				return ERR_OK;
			}
		}
		if(!f) ptr=ptr->child[ptr->n];
	}
	ptr=par;
	int old_n=ptr->n;
	while(ptr->n==2*tr->t - 1)
	{
		int m=(2*tr->t -1)/2;
		int ind=0;
		Node* parent=ptr->par;
		if(parent)
		{
			for(int j=0; j<2*tr->t; j++)
			{
				if(parent->child[j]==ptr)
				{
					ind=j;
					break;
				}
			}
		}
		else
		{
			parent=(Node*)malloc(sizeof(Node));
			parent->n=0;
			parent->keys=(int*)malloc((2*tr->t-1)*sizeof(int));
			parent->info=(Item*)calloc(2*tr->t-1, (2*tr->t-1)*sizeof(Item));
			parent->child=(Node**)calloc(2*tr->t, (2*tr->t)*sizeof(Node*));
			for(Node** pt=parent->child; pt-parent->child<2*tr->t; ++pt) *pt=NULL;
			parent->child[0]=ptr;
			parent->par=NULL;
			tr->root=parent;
		}
		Split(tr, parent, ind);
		ptr=parent;
	}
	
	//to be optimized
	if(old_n==2*tr->t -1)
	{
		par=NULL;
		while(ptr)
		{
			int f=0;
			par=ptr;
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
					Item* old=(Item*)malloc(sizeof(Item));
					old->data=ptr->info[i].data;
					old->next=ptr->info[i].next;
					ptr->info[i].data=info;
					ptr->info[i].next=old;
					return ERR_OK;
				}
			}
			if(!f) ptr=ptr->child[ptr->n];
		}
		ptr=par;
		printf("the key: %d\n", ptr->keys[0]);
	}

	int k=0;
	for(int i=ptr->n-1; i>=0; i--)
	{
		if(ptr->keys[i]>key)
		{
			ptr->keys[ptr->n - (ptr->n-1-i)]=ptr->keys[i];
			ptr->info[ptr->n - (ptr->n-1-i)]=ptr->info[i];
		}
		else
		{
			ptr->keys[i+1]=key;
			ptr->info[i+1].data=info;
			ptr->n=ptr->n+1;
			k=1;
			break;
		}
	}
	if(!k)
	{
		ptr->keys[0]=key;
		ptr->info[0].data=info;
		ptr->n=ptr->n+1;
		k=1;
	}
	
	//debug printfs
	Node* root=tr->root;
	printf("root: ");
	for(int i=0; i<root->n; i++)
	{
		printf("%d ", root->keys[i]);
	}
	printf("\n");
	for(int i=0; i<2*tr->t; i++)
	{
		Node* ch=root->child[i];
		if(!ch) continue;
		printf("ch%d: ", i);
		for(int j=0; j<ch->n; j++)
		{
			printf("%d ", ch->keys[j]);
		}
		printf("\n");
	}
	return ERR_OK;
}

void Split(Btree* tr, Node* x, int i)
{	
	Node* ptr=x->child[i];
	Node* z=(Node*)malloc(sizeof(Node));
	z->n=0;
	z->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
	z->info=(Item*)malloc((2*tr->t -1)*sizeof(Item));
	z->child=(Node**)calloc(2*tr->t, 2*tr->t*sizeof(Node*));
	for(Node** gr=z->child; gr-z->child<2*tr->t; ++gr) *gr=NULL;
	z->par=x;
	
	//will be remade with pointer iteration
	for(int j=2*tr->t-1; j>i+1; j--)
	{
		x->child[j]=x->child[j-1];
	}
	x->child[i+1]=z;
	for(int j=1+(2*tr->t -1)/2; j<ptr->n; j++)
	{
		z->keys[j-(1+(2*tr->t -1)/2)]=ptr->keys[j];
		z->info[j-(1+(2*tr->t -1)/2)]=ptr->info[j];
		z->n=z->n+1;
		ptr->n=ptr->n-1;
	}
	
	if(x->n!=0)
	{
		for(int j=2*tr->t - 2; j>i; j--)
		{
			x->keys[j]=x->keys[x->n -(2*tr->t -2 -j+1)];
			x->info[j]=x->info[x->n -(2*tr->t -2 -j+1)];
		}
	}
	x->keys[i] = ptr->keys[(2*tr->t -1)/2];
	printf("xkey: %d\n", x->keys[i]);
	x->info[i] = ptr->info[(2*tr->t -1)/2];
	x->n=x->n+1;
	ptr->n=ptr->n-1;
}

int show(Btree* tr, int level)
{
	Node* ptr=tr->root;
	if(!ptr) return 0;
	for(int i=2*tr->t-1; i>0; i--)
	{
		Btree* ntr=(Btree*)malloc(sizeof(Btree));
		ntr->t=tr->t;
		ntr->root=ptr->child[i];
		show(ntr, level+1);
		free(ntr);
		for(int j=0; j<level; j++) printf("    ");
		if((ptr->n - (2*tr->t - i)) >=0)printf("%d\n", ptr->keys[ptr->n - (2*tr->t - i)]);
	}
	return 0;
	/*if(!tr->root) return 0;
	for(int i=0; i<2*tr->t; i++)
	{
		Node* ptr=tr->root->child[i];
		if(ptr)
		{
			printf("[ ");
			for(int j=0; j<ptr->n; j++)
			{
				printf("%d ", ptr->keys[j]);
			}
			printf("] ");
		}
		else
		{
			printf("[ ");
			for(int j=0; j<2*tr->t -1; j++) printf(" ");
			printf("] ");
		}
	}
	printf("\n");
	for(int i=0; i<2*tr->t; i++)
	{
		Btree* ntr=(Btree*)malloc(sizeof(Btree));
		ntr->t=tr->t;
		ntr->root=tr->root->child[i];
		show(ntr);
		free(ntr);
	}
	return 0;
	Node* root=tr->root;
	printf("[ ")
	for(int i=0; i<root->n; i++)
	{
		printf("%d ", root->keys[i]);
	}
	printf("]");
	if(!root->par) printf("\n");
	for(int i=0; i<2*tr->t; i++)
	{
		for(int j=0; j<root->child[i]->n; j++)
		{

		}
		Btree* ntr=(Btree*)malloc(sizeof(Btree));
		ntr->t=tr->t;
		ntr->root=root->child[i];
		show(ntr);
		free(ntr);
		printf("\n");
	}*/
}





















