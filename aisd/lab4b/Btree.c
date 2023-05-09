#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"
#include "funcs.h"

Btree* InitBtree(int t)
{
	Btree* tr=(Btree*)malloc(sizeof(Btree));
	tr->t=t;
	tr->root=(Node*)malloc(sizeof(Node));
	Node* root=tr->root;
	root->n=0;
	root->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
	root->info=(Item*)calloc(2*tr->t -1, (2*tr->t -1)*sizeof(Item));
	root->child=(Node**)calloc(2*tr->t, (2*tr->t)*sizeof(Node*));
	//for(Node** gr=root->child; gr-root->child<2*tr->t; ++gr) *gr=NULL;
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
	Node* zi=NULL;
	while(ptr->n==2*tr->t - 1)
	{
		int m=(2*tr->t -1)/2;
		int ind=0;
		
		/*printf("Insertion node children before split:\n");
		for(int j=0; j<2*tr->t; j++)
		{
			Node* ch=ptr->child[j];
			printf("ch%d: ", j);
			if(!ch)
			{
				printf("NULL\n");
				continue;
			}
			else
			{
				for(int u=0; u<ch->n; u++)
				{
					printf("%d ", ch->keys[u]);
				}
				printf("\n");
			}
		}*/
		Node* parent=NULL;
		if(ptr->par)
		{
			parent=ptr->par;
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
			ptr->par=parent;
		}
	//	Node* ggr=parent;
	//	while(ggr->n==2*tr->t-1)
	//	{
	//		Node* ggpr=NULL;
	//		int indg=0;
	//		if(ggr->par)
	//		{
	//			ggpr=ggr->par;
	//			for(int j=0; j<2*tr->t; j++)
	//			{
	//				if(ggpr->child[j]==ggr)
	//				{
	//					indg=j;
	//					break;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			ggpr=(Node*)malloc(sizeof(Node));
	//			ggpr->n=0;
	//			ggpr->keys=(int*)malloc((2*tr->t-1)*sizeof(int));
	//			ggpr->info=(Item*)calloc(2*tr->t-1, (2*tr->t-1)*sizeof(Item));
	//			ggpr->child=(Node**)calloc(2*tr->t, (2*tr->t)*sizeof(Node*));
	//			//for(Node** pt=ggpr->child; pt-ggpr->child<2*tr->t; ++pt) *pt=NULL;
	//			ggpr->child[0]=ggr;
	//			ggpr->par=NULL;
	//			tr->root=ggpr;
	//			ggr->par=ggpr;
	//		}
	//		Split(tr, ggpr, indg);
	//		ggr=ggpr;
	//	}
		zi=Split(tr, parent, ind);
		printf("tr->root value: %d\n", *tr->root->keys);
		ptr=parent;
	}
	
	//to be optimized
	if(old_n==2*tr->t -1)
	{
		ptr=zi->par;
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
		//printf("the key: %d\n", ptr->keys[0]);
	}
	//printf("ptr->n: %d\n", ptr->n);
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
	/*Node* root=tr->root;
	printf("root: ");
	for(int i=0; i<root->n; i++)
	{
		printf("%d ", root->keys[i]);
	}
	printf("\n");
	for(int i=0; i<2*tr->t; i++)
	{
		Node* ch=root->child[i];
		printf("ch%d: ", i);
		if(!ch) 
		{
			printf("NULL\n");
			continue;
		}
		for(int j=0; j<ch->n; j++)
		{
			printf("%d ", ch->keys[j]);
		}
		printf("\n");
	}*/
	return ERR_OK;
}

Node* Split(Btree* tr, Node* x, int i)
{	
	//Node* ptr=x->child[i];
	Node* z=(Node*)malloc(sizeof(Node));
	z->n=0;
	z->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
	z->info=(Item*)malloc((2*tr->t -1)*sizeof(Item));
	z->child=(Node**)calloc(2*tr->t, 2*tr->t*sizeof(Node*));
	//for(Node** gr=z->child; gr-z->child<2*tr->t; ++gr) *gr=NULL;
	z->par=x;
	Node* nx=x;
	if(x->n==2*tr->t-1)
	{
		Node* ggpr=NULL;
		int indg=0;
		if(x->par)
		{
			ggpr=x->par;
			for(int j=0; j<2*tr->t; j++)
			{
				if(ggpr->child[j]==x)
				{
					indg=j;
					break;
				}
			}
		}
		else
		{
			ggpr=(Node*)malloc(sizeof(Node));
			ggpr->n=0;
			ggpr->keys=(int*)malloc((2*tr->t-1)*sizeof(int));
			ggpr->info=(Item*)calloc(2*tr->t-1, (2*tr->t-1)*sizeof(Item));
			ggpr->child=(Node**)calloc(2*tr->t, (2*tr->t)*sizeof(Node*));
			//for(Node** pt=ggpr->child; pt-ggpr->child<2*tr->t; ++pt) *pt=NULL;
			ggpr->child[0]=x;
			ggpr->par=NULL;
			tr->root=ggpr;
			x->par=ggpr;
		}
		nx=Split(tr, ggpr, indg);
	}
	Node* ptr=nx->child[i];
	//printf("z->keys child 0: %d\n", *nx->child[0]->keys);
	if(nx!=x) 
	{
		ptr=nx->child[0];
		i=0;
	}
	z->par=nx;
	//printf("ptr->key: %d\n", *ptr->keys);
	/*printf("x keys: ");
	for(int j=0; j<x->n; j++)
	{
		printf("%d ", x->keys[j]);
	}
	printf("\n");
	printf("Children before split:\n");
	for(int j=0; j<2*tr->t; j++)
	{
		Node* ch=x->child[j];
		printf("ch%d: ", j);
		if(!ch)
		{
			printf("NULL\n");
			continue;
		}
		else
		{
			for(int u=0; u<ch->n; u++)
			{
				printf("%d ", ch->keys[u]);
			}
			printf("\n");
		}
	}*/

	//will be remade with pointer iteration
	for(int j=2*tr->t-1; j>i+1; j--)
	{
		nx->child[j]=nx->child[j-1];
	}
	nx->child[i+1]=z;
	for(int j=1+(2*tr->t -1)/2; j<ptr->n; j++)
	{
		z->keys[j-(1+(2*tr->t -1)/2)]=ptr->keys[j];
		z->info[j-(1+(2*tr->t -1)/2)]=ptr->info[j];
		z->child[j-(1+(2*tr->t -1)/2)]=ptr->child[j];
		if(ptr->child[j]) ptr->child[j]->par=z;
		ptr->child[j]=NULL;
		z->n=z->n+1;
	}
	//printf("z->key: %d\n", *z->keys);
	z->child[ptr->n-(1+(2*tr->t -1)/2)]=ptr->child[ptr->n];
	if(ptr->child[ptr->n]) ptr->child[ptr->n]->par=z;
	ptr->n=ptr->n-z->n;
	if(nx->n!=0)
	{
		for(int j=nx->n; j>i; j--) //2*tr->t - 2
		{
			nx->keys[j]=nx->keys[nx->n -(nx->n -j+1)];
			nx->info[j]=nx->info[nx->n -(nx->n -j+1)];
		}
	}
	nx->keys[i] = ptr->keys[(2*tr->t -1)/2];
	//printf("xkey: %d\n", x->keys[i]);
	//printf("x->n: %d\n", x->n);
	nx->info[i] = ptr->info[(2*tr->t -1)/2];
	nx->n=nx->n+1;
	ptr->n=ptr->n-1;
	return z;
}

int DelNode(Btree* tr, int key, int rel)
{
	Node* ptr=tr->root;
	while(ptr)
	{
		int pos=-1;
		int ci=ptr->n;
		for(int u=0; u<ptr->n; u++)
		{
			if(ptr->keys[u]==key)
			{
				pos=u;
				break;
			}
			if(ptr->keys[u]>key)
			{
				ci=u;
				break;
			}
		}
		if(pos==-1 && !*ptr->child) return ERR_NF;
		if(pos!=-1)
		{
			if(ptr->info[pos].next)
			{
				int u=pos;
				if(rel==1)
				{
					char* grd=ptr->info[u].data;
					Item* sec=ptr->info[u].next;
					ptr->info[u].data=ptr->info[u].next->data;
					ptr->info[u].next=ptr->info[u].next->next;
					free(grd);
					free(sec);
					return ERR_OK;
				}
				else
				{
					Item* gr=ptr->info+u;
					Item* prev=ptr->info+u;
					int y=0;
					while(gr)
					{
						y++;
						if(y==rel)
						{
							prev->next=gr->next;
							gr->next=NULL;
							free(gr->data);
							free(gr);
							return ERR_OK;
						}
						prev=gr;
						gr=gr->next;
					}
					return ERR_NF;
				}
			}
			else if(rel==1)
			{
				int u=pos;
				//printf("condition: %d\n", *(ptr->child)==NULL);
				if(*(ptr->child)==NULL)
				{
					free(ptr->info[u].data);
					for(int j=u; j<ptr->n-1; j++)
					{
						ptr->keys[j]=ptr->keys[j+1];
						ptr->info[j]=ptr->info[j+1];
					}
					ptr->n=ptr->n-1;
					return ERR_OK;
				}
				if(ptr->child[u]->n>=tr->t)
				{
					Node* pk=ptr->child[u];
					while(*pk->child)
					{
						pk=pk->child[pk->n];
					}
					int k_d=pk->keys[pk->n-1];
					Item* i_d=pk->info+pk->n-1;
					pk->n=pk->n-1;
					ptr->keys[u]=k_d;
					free(ptr->info[u].data);
					//free(ptr->info+u);
					ptr->info[u]=*i_d;
					return ERR_OK;
				}
				else if(ptr->child[u+1]->n>=tr->t)
				{
					Node* pk=ptr->child[u+1];
					while(*pk->child)
					{
						pk=*pk->child;
					}
					int k_d=*pk->keys;
					Item* i_d=pk->info;
					for(int j=0; j<pk->n-1; j++)
					{
						pk->keys[j]=pk->keys[j+1];
						pk->info[j]=pk->info[j+1];
					}
					pk->n=pk->n-1;
					ptr->keys[u]=k_d;
					free(ptr->info[u].data);
					//free(ptr->info+u);
					ptr->info[u]=*i_d;
					return ERR_OK;
				}
				else
				{
					Item info=ptr->info[u];
					Node* y=ptr->child[u];
					Node* z=ptr->child[u+1];
					
					MergeNode(y, z);
					//removing key from ptr
					for(int j=u; j<ptr->n-1; j++)
					{
						ptr->keys[j]=ptr->keys[j+1];
						ptr->info[j]=ptr->info[j+1];
					}
					
					for(int j=u+1; j<ptr->n; j++)
					{
						ptr->child[j]=ptr->child[j+1];
					}
					ptr->n=ptr->n-1;
					if(ptr==tr->root && !ptr->n) 
					{
						tr->root=y;
						for(int j=0; j<=y->n; j++)
						{
							y->child[j]->par=y;
						}
						y->par=NULL;
					}
				//	if(ptr==tr->root)
				//	{
				//		tr->root=y;
				//		y->par=NULL;
				//	}
					return ERR_OK;
				}
			}
			else return ERR_IR;
		}
		else
		{
			if(ptr->child[ci]->n>=tr->t)
			{
				Btree* ntr=(Btree*)malloc(sizeof(Btree));
				ntr->t=tr->t;
				ntr->root=ptr->child[ci];
				int res=DelNode(ntr, key, rel);
				free(ntr);
				return res;
			}
			else 
			{
				if(ci-1>=0)
				{
					if(ptr->child[ci-1]->n>=tr->t)
					{
						Node* y=ptr->child[ci-1];
						Node* z=ptr->child[ci];

						for(int j=z->n; j>0; j--)
						{
							z->keys[j]=z->keys[j-1];
							z->info[j]=z->info[j-1];
							z->child[j+1]=z->child[j];
						}
						z->child[z->n]=z->child[z->n-1];
						*z->keys=ptr->keys[ci-1];
						*z->info=ptr->info[ci-1];
						z->n=z->n+1;
						ptr->keys[ci-1]=y->keys[y->n-1];
						ptr->info[ci-1]=y->info[y->n-1];
						y->n=y->n-1;
						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=z;
						int res=DelNode(ntr, key, rel);
						free(ntr);
						return res;
					}
				}
				if(ci+1<=ptr->n)
				{
					if(ptr->child[ci+1]->n>=tr->t)
					{
						Node* y=ptr->child[ci];
						Node* z=ptr->child[ci+1];

						y->keys[y->n]=ptr->keys[ci];
						y->info[y->n]=ptr->info[ci];
						y->child[y->n+1]=*z->child;
						y->n=y->n+1;

						ptr->keys[ci]=*z->keys;
						ptr->info[ci]=*z->info;
						
						for(int j=0; j<z->n-1; j++)
						{
							z->keys[j]=z->keys[j+1];
							z->info[j]=z->info[j+1];
							z->child[j]=z->child[j+1];
						}
						z->child[z->n-1]=z->child[z->n];
						z->n=z->n-1;
						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=y;
						int res=DelNode(ntr, key, rel);
						free(ntr);
						return res;
					}
				}
				if(ci-1>=0)
				{
					Node* y=ptr->child[ci-1];
					Node* z=ptr->child[ci];

					y->keys[y->n]=ptr->keys[ci-1];
					y->info[y->n]=ptr->info[ci-1];
					y->n=y->n+1;

					/*for(int j=0; j<z->n; j++)
					{
						y->keys[y->n]=z->keys[j];
						y->info[y->n]=z->info[j];
						y->child[y->n]=z->child[j];
						y->n=y->n+1;
					}
					y->child[y->n]=z->child[z->n];
					z->n=0;*/
					MergeNode(y, z);

					for(int j=ci-1; j<ptr->n-1; j++)
					{
						ptr->keys[j]=ptr->keys[j+1];
						ptr->info[j]=ptr->info[j+1];
						ptr->child[j+1]=ptr->child[j+2];
					}
					ptr->n=ptr->n-1;
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=y;
					printf("ptr value: %d\n", *ptr->keys);
					if(ptr==tr->root && !ptr->n) 
					{
						tr->root=y;
						for(int j=0; j<=y->n; j++)
						{
							y->child[j]->par=y;
						}
						y->par=NULL;
					}
					printf("tr->root value: %d\n", *tr->root->keys);
					int res=DelNode(ntr, key, rel);
					free(ntr);
					return res;
				}
				if(ci+1<=ptr->n)
				{
					Node* y=ptr->child[ci];
					Node* z=ptr->child[ci+1];

					y->keys[y->n]=ptr->keys[ci];
					y->info[y->n]=ptr->info[ci];
					y->n=y->n+1;

					/*for(int j=0; j<z->n; j++)
					{
						y->keys[y->n]=z->keys[j];
						y->info[y->n]=z->info[j];
						y->child[y->n]=z->child[j];
						y->n=y->n+1;
					}
					y->child[y->n]=z->child[z->n];
					z->n=0;*/
					MergeNode(y, z);

					for(int j=ci; j<ptr->n-1; j++)
					{
						ptr->keys[j]=ptr->keys[j+1];
						ptr->info[j]=ptr->info[j+1];
						ptr->child[j+1]=ptr->child[j+2];
					}
					ptr->n=ptr->n-1;
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=y;
					printf("ptr value: %d\n", *ptr->keys);
					if(ptr==tr->root && !ptr->n)
					{
						tr->root=y;
						for(int j=0; j<=y->n; j++)
						{
							y->child[j]->par=y;
						}
						y->par=NULL;
					}
					printf("tr->root value: %d\n", *tr->root->keys);
					int res=DelNode(ntr, key, rel);
					free(ntr);
					return res;	
				}
			}
		}
		ptr=ptr->child[ci];
	}
}

void MergeNode(Node* y, Node* z)
{	
	
	//moving all keys and children from z node to y node
	for(int j=0; j<z->n; j++)
	{
		y->keys[y->n]=z->keys[j];
		y->info[y->n]=z->info[j];
		if(y->child[y->n])
		{
			MergeNode(y->child[y->n], z->child[j]);
		}
		else y->child[y->n]=z->child[j];
		y->n=y->n+1;
	}
	y->child[y->n]=z->child[z->n];
	z->n=0;
	
	//removing z
	
	//erase node function for z. Now in developement
}

void show(Btree* tr, int level)
{
	Node* ptr=tr->root;
	if(ptr)
	{
		for(int i=ptr->n; i>=0; i--)
		{
			Btree* ntr=(Btree*)malloc(sizeof(Btree));
			ntr->t=tr->t;
			ntr->root=ptr->child[i];
			show(ntr, level+1);
			free(ntr);
			//printf("level: %d\n", level);
			for(int j=0; j<level; j++) printf("    ");
			if((ptr->n - (ptr->n - i+1)) >=0) printf("%d\n", ptr->keys[ptr->n - (ptr->n - i+1)]);
			printf("\n");
		}
	}
	
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

void fimport(Btree* tr, char* fn)
{
	FILE* fd=fopen(fn, "r");
	while(!feof(fd))
	{
		int key=-1;
		fscanf(fd, "%d\n", &key);
		if(key==-1) break;
		char* info=enterf(fd);
		AddNode(tr, key, info);
	}
	fclose(fd);
}

//garbage that could be useful


/*if(y->n-1<0)
{
*y->keys=key;
*y->info=info;
}
for(int j=y->n-1; j>=0; j--)
{
	if(y->keys[j]>key)
	{
		y->keys[j+1]=y->keys[j];
		y->info[j+1]=y->info[j];
	}
	else
	{
		y->keys[j+1]=key;
		y->keys[j+1]=info;
	}
}
y->n=y->n+1;*/


