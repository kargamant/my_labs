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
	ntr->root=ptr->child[2*tr->t -1];
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

	
	//searching for a leaf to insert
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
				//Item* ins=(Item*)malloc(sizeof(Item));
				//ins->data=info;
				Item* old=(Item*)malloc(sizeof(Item));
				old->data=ptr->info[i].data;
				old->next=ptr->info[i].next;
				ptr->info[i].data=info;
				ptr->info[i].next=old;
				//Item* head=ptr->info+i;
				//ins->next=head;
				//ptr->info[i]=*ins;
				return ERR_OK;
			}
		}
		if(!f) 
		{
			ptr=ptr->child[ptr->n];
		}
	}
	ptr=par;
	if(ptr->n==2*tr->t-1)
	{
		int ind=0;
		if(ptr->par)
		{
			for(int j=0; j<=ptr->par->n; j++)
			{
				if(ptr->par->child[j]==ptr)
				{
					ind=j;
					break;
				}
			}
		}
		else
		{	
			ptr->par=(Node*)malloc(sizeof(Node));
			ptr->par->n=0;
			ptr->par->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
			ptr->par->info=(Item*)calloc(2*tr->t -1, (2*tr->t -1)*sizeof(Item));
			ptr->par->child=(Node**)malloc((2*tr->t)*sizeof(Node*));
			for(Node** gr=ptr->par->child; gr-ptr->par->child<2*tr->t; ++gr) *gr=NULL;
			ptr->par->par=NULL;
			ptr->par->child[0]=ptr;
			tr->root=ptr->par;
		}
		Split(tr, ptr->par, ind);
		tr->root=ptr->par;

		Node* krr=tr->root;
		if(krr)
		{
			printf("[ ");
			for(int j=0; j<krr->n; j++)
			{
				printf("%d ", krr->keys[j]);
			}
			printf("] ");
		}
		else
		{
			printf("[ ");
			for(int j=0; j<2*tr->t -1; j++) printf(" ");
			printf("] ");
		}
		printf("\n");
		show(tr);
		printf("\n");
		
		ptr=tr->root;
		par=NULL;
		
		//searching for a leaf to insert
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
					//Item* ins=(Item*)malloc(sizeof(Item));
					//ins->data=info;
					Item* old=(Item*)malloc(sizeof(Item));
					old->data=ptr->info[i].data;
					old->next=ptr->info[i].next;
					ptr->info[i].data=info;
					ptr->info[i].next=old;
					//Item* head=ptr->info+i;
					//ins->next=head;
					//ptr->info[i]=*ins;
					return ERR_OK;
				}
			}
			if(!f) 
			{
				ptr=ptr->child[ptr->n];
			}
		}
		ptr=par;
		int k=0;
		for(int i=ptr->n; i>=0; i--)
		{
			if(ptr->keys[i-1]>key)
			{
				ptr->keys[i]=ptr->keys[i-1];
				ptr->info[i]=ptr->info[i-1];
			}
			else
			{
				ptr->keys[i]=key;
				ptr->info[i].data=info;
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
		}
		/*
		if(ptr->par->child[ind]->keys[-1+(2*tr->t -1)/2]>key)
		{
			ptr->par->child[ind-1]->keys[(2*tr->t -1)/2]=key;
			ptr->par->child[ind-1]->info[(2*tr->t -1)/2].data=info;
		}
		else
		{
			ptr->par->child[ind]->keys[(2*tr->t -1)/2]=key;
			ptr->par->child[ind]->info[(2*tr->t -1)/2].data=info;
		}*/
	}
	else if(ptr->n==0)
	{
		ptr->keys[0]=key;
		ptr->info[0].data=info;
		ptr->info[0].next=NULL;
		ptr->n=ptr->n+1;
	}
	else
	{
		int k=0;
		for(int i=ptr->n; i>=0; i--)
		{
			if(ptr->keys[i-1]>key)
			{
				ptr->keys[i]=ptr->keys[i-1];
				ptr->info[i]=ptr->info[i-1];
			}
			else
			{
				ptr->keys[i]=key;
				ptr->info[i].data=info;
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
		}
	}
	return ERR_OK;
}

void Split(Btree* tr, Node* x, int i)
{	
	/*if(!x)
	{
		x=(Node*)malloc(sizeof(Node));
		x->n=0;
		x->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
		x->info=(Item*)malloc((2*tr->t -1)*sizeof(Item));
		x->child=(Node**)malloc(2*tr->t*sizeof(Node*));
		for(Node** rt=x->child; rt-x->child<2*tr->t; ++rt) *rt=NULL;
		x->child[i]=tr->root;
		x->par=NULL;
		tr->root->par=x;
		tr->root=x;
	}*/
	Node* ptr=x->child[i];
	Node* z=(Node*)malloc(sizeof(Node));
	z->n=0;
	z->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
	z->info=(Item*)malloc((2*tr->t -1)*sizeof(Item));
	z->child=(Node**)malloc(2*tr->t*sizeof(Node*));
	for(Node** gr=z->child; gr-z->child<2*tr->t; ++gr) *gr=NULL;
	z->par=x;
	
	//will be remade with pointer iteration
	for(int j=x->n+1; j>i+1; j--)
	{
		x->child[j]=x->child[j-1];
	}
	x->child[i+1]=z;
	for(int j=1+(2*tr->t -1)/2; j<ptr->n; j++)
	{
		z->keys[j-(1+(2*tr->t -1)/2)]=ptr->keys[j];
		z->info[j-(1+(2*tr->t -1)/2)]=ptr->info[j];
		z->n=z->n+1;
	}
	if(i!=0)
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
	ptr->n=ptr->n-2;
}

int show(Btree* tr)
{
	if(!tr->root) return 0;
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
	/*Node* root=tr->root;
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





















