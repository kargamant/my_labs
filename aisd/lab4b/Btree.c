#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"
#include "funcs.h"

Btree* InitBtree(int t)
{
	Btree* tr=(Btree*)malloc(sizeof(Btree));
	tr->t=t;
	tr->root=(Node*)malloc(sizeof(Node));
	tr->root->mark=1;
	Node* root=tr->root;
	root->n=0;
	root->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
	root->info=(Item*)calloc(2*tr->t -1, (2*tr->t -1)*sizeof(Item));
	root->child=(Node**)calloc(2*tr->t, (2*tr->t)*sizeof(Node*));
	for(Node** gr=root->child; gr-root->child<2*tr->t; ++gr) *gr=NULL;
	root->par=NULL;
	//for(int i=0; i<2*tr->t; i++) root->child[i]=NULL;
	return tr;	
}

void erase(Btree* tr)
{
	Node* ptr=tr->root;
	if(ptr) //ptr->n<=3 && ptr->n>=1
	{
		for(int i=0; i<=ptr->n; i++)
		{
			Btree* ntr=(Btree*)malloc(sizeof(Btree));
			ntr->t=tr->t;
			ntr->root=ptr->child[i];
			//	ntr->root->n=ptr->child[i]->n;
			//	ntr->root->keys=ptr->child[i]->keys;
			//	ntr->root->info=ptr->child[i]->info;
			//	ntr->root->child=ptr->child[i]->child;
			//	ntr->root->par=ptr->child[i]->par;
			erase(ntr);
			free(ntr);
			if(i<ptr->n)
			{
				Item* gr=ptr->info+i;
				int k=0;
				while(gr)
				{
					k++;
					if(gr->data) 
					{
						free(gr->data);
						gr->data=NULL;
					}
					Item* next=gr->next;
					if(gr && k>1) 
					{
						free(gr);
						gr=NULL;
					}
					gr=next;
				}
			}
		}
		free(ptr->keys);
		free(ptr->info);
		//for(int i=0; i<2*tr->t; i++) ptr->child[i]=NULL;
		//printf("db1\n");
		free(ptr->child);
		ptr->keys=NULL;
		ptr->info=NULL;
		ptr->child=NULL;
		//printf("db2\n");
		free(ptr);
		ptr=NULL;
		//printf("db3\n");
	//	if(ptr->par)
	//	{
	//		if(&ptr<ptr->par->child || &ptr>(ptr->par->child+2*tr->t-1)) free(ptr);
	//	}
	}
}

Node* Min(Btree* tr, int key)
{
	Node* ptr=tr->root;
	int mkey=2147483647;
	Node* mn=ptr;
	if(ptr)
	{
		//mkey=2147483647;
		//mn=NULL;
		//printf("ptr->n: %d\n", ptr->n);
		for(int i=0; i<ptr->n; i++)
		{
			//printf("ptr->keys[%d]: %d\n", i, ptr->keys[i]);
			if(ptr->keys[i]>key)
			{
				if(ptr->keys[i]<mkey)
				{
					mkey=ptr->keys[i];
					mn=ptr;
				}
			}
		}
		//printf("mkey: %d\n", mkey);
		for(int h=0; h<ptr->n+1; h++)
		{
			Btree* ntr=(Btree*)malloc(sizeof(Btree));
			ntr->t=tr->t;
			ntr->root=ptr->child[h];
			Node* nmn=Min(ntr, key);
			if(nmn==NULL) 
			{
				free(ntr);
				return mn;
			}
			free(ntr);
			int nmkey=2147483647;
			for(int y=0; y<nmn->n; y++)
			{
				if(nmn->keys[y]>key)
				{
					if(nmn->keys[y]<nmkey) nmkey=nmn->keys[y];
				}
			}
			if(nmkey<mkey) mn=nmn;
		}
	}
	return mn;
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
			if(!ptr->keys) return NULL;
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
			if(!ptr->keys) return ERR_NF;
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
				//if(!parent->child) return ERR_NF;
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
			parent->mark=1;
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
		if(!zi) return -1;
		//printf("tr->root value: %d\n", *tr->root->keys);
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
					//free(ptr->info[i].data);
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
			ptr->info[i+1].next=NULL;
			ptr->n=ptr->n+1;
			k=1;
			break;
		}
	}
	if(!k)
	{
		ptr->keys[0]=key;
		ptr->info[0].data=info;
		ptr->info[0].next=NULL;
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
	z->mark=1;
	z->keys=(int*)malloc((2*tr->t -1)*sizeof(int));
	z->info=(Item*)calloc(2*tr->t-1, (2*tr->t -1)*sizeof(Item));
	z->child=(Node**)calloc(2*tr->t, 2*tr->t*sizeof(Node*));
	for(Node** gr=z->child; gr-z->child<2*tr->t; ++gr) *gr=NULL;
	z->par=x;
	Node* nx=x;
	int indg=0;
	if(x->n==2*tr->t-1)
	{
		Node* ggpr=NULL;
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
			for(Node** pt=ggpr->child; pt-ggpr->child<2*tr->t; ++pt) *pt=NULL;
			ggpr->child[0]=x;
			ggpr->par=NULL;
			tr->root=ggpr;
			x->par=ggpr;
		}
		nx=Split(tr, ggpr, indg);
		if(!nx) return NULL;
	}
	Node* ptr=NULL;
	//printf("z->keys child 0: %d\n", *nx->child[0]->keys);
	if(nx!=x) 
	{
		if(nx->child[0] && i==2)
		{
			//if(nx->child[0]->n==2*tr->t-1)
			ptr=nx->child[0];
			i=0;
		}
		else if(nx->child[1] && i==3)
		{
			ptr=nx->child[1];
			i=1;
		}
		else
		{
			if(!nx || !nx->par || !nx->par->child[indg]) ptr=NULL;
			else
			{
				ptr=nx->par->child[indg]->child[i];
				nx=nx->par->child[indg];
			}
		}
	}
	else ptr=nx->child[i];
	if(!ptr) return NULL;
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
	for(int j=nx->n+1; j>i+1; j--) //2*tr->t-1
	{
		nx->child[j]=nx->child[j-1];
	}
	nx->child[i+1]=z;
	for(int j=1+(2*tr->t -1)/2; j<ptr->n; j++)
	{
		z->keys[j-(1+(2*tr->t -1)/2)]=ptr->keys[j];
		if(ptr->info[j].data) 
		{
			z->info[j-(1+(2*tr->t -1)/2)].data=strdup(ptr->info[j].data);
			free(ptr->info[j].data);
			ptr->info[j].data=NULL;
		}
		else z->info[j-(1+(2*tr->t -1)/2)].data=ptr->info[j].data;
		z->info[j-(1+(2*tr->t -1)/2)].next=ptr->info[j].next;
		z->child[j-(1+(2*tr->t -1)/2)]=ptr->child[j];
		if(ptr->child[j]) ptr->child[j]->par=z;
		else ptr->child[j]=NULL;
		z->n=z->n+1;
	}
	//printf("z->key: %d\n", *z->keys);
	z->child[ptr->n-(1+(2*tr->t -1)/2)]=ptr->child[ptr->n];
	if(ptr->child[ptr->n]) ptr->child[ptr->n]->par=z;
	//for(int j=ptr->n; j<z->n; j++) ptr->keys[j]=-1
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
	if(ptr->info[(2*tr->t -1)/2].data)
	{
		nx->info[i].data = strdup(ptr->info[(2*tr->t -1)/2].data);
		free(ptr->info[(2*tr->t -1)/2].data);
		ptr->info[(2*tr->t -1)/2].data=NULL;
	}
	nx->info[i].next = ptr->info[(2*tr->t -1)/2].next;
	nx->n=nx->n+1;
	ptr->n=ptr->n-1;
	ptr->keys[ptr->n]=-1;
	//printf("z->n: %d\n", z->n);
	return z;
}

/*void Afterd(Node* ptr, Btree* tr)
{
	Btree* gtr=(Btree*)malloc(sizeof(Btree));
	gtr->t=tr->t;
	gtr->root=ptr->child[ptr->n];
	erase(gtr);
	free(gtr);
	ptr->child[ptr->n]=NULL;
}*/

/*int DelNode(Btree* tr, int key, int rel)
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
			//printf("ptr->n: %d\n", ptr->n);
			//printf("pos: %d\n", pos);
			//printf("ptr->keys: ");
			//for(int i=0; i<2*tr->t-1; i++) printf("%d ", ptr->keys[i]);
			//printf("\n");
			if(ptr->info[pos].next)
			{
				int u=pos;
				if(rel==1)
				{
					free(ptr->info[u].data);
					Item* sec=ptr->info[u].next;
					ptr->info[u].data=ptr->info[u].next->data;
					ptr->info[u].next=ptr->info[u].next->next;
					free(sec);
					sec=NULL;
					return ERR_OK;
				}
				else
				{
					Item* gr=ptr->info+u;
					Item* prev=ptr->info+u;
					int y=0;
					//printf("db1\n");
					while(gr)
					{
						y++;
						if(y==rel)
						{
							prev->next=gr->next;
							gr->next=NULL;
							free(gr->data);
							gr->data=NULL;
							free(gr);
							gr=NULL;
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
			//	for(int i=0; i<ptr->n; i++) printf("%d ", ptr->keys[i]);
			//	printf("\n");
			//	for(int i=0; i<ptr->n; i++) printf("\"%s\" ", ptr->info[i].data);
			//	printf("\n");
				int u=pos;
				//printf("condition: %d\n", *(ptr->child)==NULL);
				if(*ptr->child==NULL && *(ptr->child+1)==NULL)
				{
					free(ptr->info[u].data);
					ptr->info[u].data=NULL;
					for(int j=u; j<ptr->n-1; j++)
					{
						ptr->keys[j]=ptr->keys[j+1];
						ptr->info[j]=ptr->info[j+1];
					}
					ptr->n=ptr->n-1;
					ptr->keys[ptr->n]=-1;
					
					//Afterd(ptr, tr);
					return ERR_OK;
				}
				if(ptr->child[u]->n>=tr->t)
				{
					Node* pk=ptr->child[u];
					//Node* prev=ptr->child[u];
					while(pk->child[pk->n])
					{
						//prev=pk;
						pk=pk->child[pk->n];
					}
					int k_d=pk->keys[pk->n-1];
					Item i_d=pk->info[pk->n-1];
					pk->n=pk->n-1;
					pk->keys[pk->n]=-1;
					
					//Afterd(pk, tr);

					ptr->keys[u]=k_d;
					free(ptr->info[u].data);
					ptr->info[u].data=NULL;
					//free(ptr->info+u);
					ptr->info[u].data=strdup(i_d.data);
					free(i_d.data);
					i_d.data=NULL;
					ptr->info[u].next=i_d.next;
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=ptr->child[u];
					DelNode(ntr, k_d, 1);
					free(ntr);
					return ERR_OK;
				}
				else if(ptr->child[u+1]->n>=tr->t)
				{
					Node* pk=ptr->child[u+1];
					Node* prev=ptr->child[u+1];
					while(*pk->child)
					{
						prev=pk;
						pk=*pk->child;
					}
					int k_d=*pk->keys;
					Item i_d=*pk->info;
					for(int j=0; j<pk->n-1; j++)
					{
						pk->keys[j]=pk->keys[j+1];
						pk->info[j]=pk->info[j+1];
					}
					pk->n=pk->n-1;
					pk->keys[pk->n]=-1;
					
					//Afterd(pk, tr);

					ptr->keys[u]=k_d;
					free(ptr->info[u].data);
					ptr->info[u].data=NULL;
					//free(ptr->info+u);
					ptr->info[u]=i_d;
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=prev;
					DelNode(ntr, k_d, 1);
					free(ntr);
					return ERR_OK;
				}
				else
				{
					Item info=ptr->info[u];
					Node* y=ptr->child[u];
					Node* z=ptr->child[u+1];
					
					MergeNode(y, z, tr);

					//freeing ptr->info[u]
					free(ptr->info[u].data);

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
					ptr->keys[ptr->n]=-1;
					//Afterd(ptr, tr);

					if(ptr==tr->root && !ptr->n) 
					{
						free(ptr->keys);
						free(ptr->child);
						free(ptr->info);
						y->par=tr->root->par;
						tr->root=y;
						free(ptr);
						ptr=NULL;
					}
				//	if(ptr==tr->root && !ptr->n)
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

						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=ptr->child[ci];
						Item* gh=ptr->info+ci-1;
						int m=0;
						while(gh)
						{
							m++;
							AddNode(ntr, ptr->keys[ci-1], gh->data);
							Item* next=gh->next;
							if(m!=1) free(gh);
							gh=next;
						}
						
						Node* pk=y;
						while(pk->child[pk->n])
						{
							pk=pk->child[pk->n];
						}
						y=pk;
						ptr->keys[ci-1]=y->keys[y->n-1];
						if(y->info[y->n-1].data) ptr->info[ci-1].data=strdup(y->info[y->n-1].data);
						else ptr->info[ci-1].data=y->info[y->n-1].data;
						if(!y->info[y->n-1].next) ptr->info[ci-1].next=y->info[y->n-1].next;
						else ptr->info[ci-1].next=NULL;
						
						ntr->root=ptr->child[ci-1];
						DelNode(ntr, y->keys[y->n-1], 1);
						ntr->root=ptr->child[ci];
						int res=DelNode(ntr, key, rel);
						free(ntr);
						return res;
						for(int j=z->n; j>0; j--)
						{
							z->keys[j]=z->keys[j-1];
							z->info[j]=z->info[j-1];
							z->child[j+1]=z->child[j];
						}
						z->child[z->n]=z->child[z->n-1];
						*z->child=y->child[y->n];
						y->child[y->n]=NULL;
						*z->keys=ptr->keys[ci-1];
						z->info[0].data=strdup(ptr->info[ci-1].data);
						z->info[0].next=ptr->info[ci-1].next;
						free(ptr->info[ci-1].data);
						z->n=z->n+1;
						ptr->keys[ci-1]=y->keys[y->n-1];
						ptr->info[ci-1].data=strdup(y->info[y->n-1].data);
						ptr->info[ci-1].next=y->info[y->n-1].next;
						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=ptr->child[ci-1];
						DelNode(ntr, y->keys[y->n-1], 1);
						//free(ntr);
						//y->n=y->n-1;
						//ntr=(Btree*)malloc(sizeof(Btree));
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

						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=y;
						Item* gh=ptr->info+ci;
						int m=0;
						while(gh)
						{
							m++;
							AddNode(ntr, ptr->keys[ci], gh->data);
							Item* next=gh->next;
							if(m!=1) free(gh);
							gh=next;
						}
						//AddNode(ntr, ptr->keys[ci], ptr->info[ci].data);

						Node* pk=z;
						while(pk->child[0])
						{
							pk=pk->child[0];
						}
						z=pk;
						ptr->keys[ci]=z->keys[0];
						ptr->info[ci].data=strdup(z->info[0].data);
						ptr->info[ci].next=z->info[0].next;
						
						ntr->root=ptr->child[ci+1];
						DelNode(ntr, z->keys[0], 1);
						ntr->root=y;
						int res=DelNode(ntr, key, rel);
						free(ntr);
						return res;
						y->keys[y->n]=ptr->keys[ci];
						y->info[y->n].data=strdup(ptr->info[ci].data);
						y->info[y->n].next=ptr->info[ci].next;
						free(ptr->info[ci].data);
						y->child[y->n+1]=*z->child;
						y->n=y->n+1;

						ptr->keys[ci]=*z->keys;
						ptr->info[ci].data=strdup(z->info[0].data);
						free(z->info[0].data);
						ptr->info[ci].next=z->info[0].next;
						
						for(int j=0; j<z->n-1; j++)
						{
							z->keys[j]=z->keys[j+1];
							z->info[j]=z->info[j+1];
							z->child[j]=z->child[j+1];
						}
						z->child[z->n-1]=z->child[z->n];
						z->n=z->n-1;
						z->keys[z->n]=-1;
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
					//y->child[y->n+1]=z->child[0];
					y->n=y->n+1;

					for(int j=0; j<z->n; j++)
					{
						y->keys[y->n]=z->keys[j];
						y->info[y->n]=z->info[j];
						y->child[y->n]=z->child[j];
						y->n=y->n+1;
					}
					y->child[y->n]=z->child[z->n];
					z->n=0;
					Btree* nntr=(Btree*)malloc(sizeof(Btree));
					nntr->t=tr->t;
					nntr->root=z;
					MergeNode(y, z, nntr);
					free(nntr);

					for(int j=ci-1; j<ptr->n-1; j++)
					{
						ptr->keys[j]=ptr->keys[j+1];
						ptr->info[j]=ptr->info[j+1];
						ptr->child[j+1]=ptr->child[j+2];
					}
					ptr->n=ptr->n-1;
					ptr->keys[ptr->n]=-1;
					//Afterd(ptr, tr);
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=y;
					//printf("ptr value: %d\n", *ptr->keys);
					if(ptr==tr->root && !ptr->n) 
					{
						free(ptr->keys);
						free(ptr->child);
						free(ptr->info);
						free(ptr);
						ptr=NULL;
						tr->root=y;
						for(int j=0; j<=y->n; j++)
						{
							y->child[j]->par=y;
						}
						tr->root->par=NULL;
					}
					//printf("tr->root value: %d\n", *tr->root->keys);
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
					//y->child[y->n+1]=z->child[0];
					y->n=y->n+1;

					for(int j=0; j<z->n; j++)
					{
						y->keys[y->n]=z->keys[j];
						y->info[y->n]=z->info[j];
						y->child[y->n]=z->child[j];
						y->n=y->n+1;
					}
					y->child[y->n]=z->child[z->n];
					z->n=0;
					Btree* nntr=(Btree*)malloc(sizeof(Btree));
					nntr->t=tr->t;
					nntr->root=z;
					MergeNode(y, z, nntr);
					free(nntr);
					for(int j=ci; j<ptr->n-1; j++)
					{
						ptr->keys[j]=ptr->keys[j+1];
						ptr->info[j]=ptr->info[j+1];
						ptr->child[j+1]=ptr->child[j+2];
					}
					ptr->n=ptr->n-1;
					ptr->keys[ptr->n]=-1;
					//Afterd(ptr, tr);
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=y;
					//printf("ptr value: %d\n", *ptr->keys);
					if(ptr==tr->root && !ptr->n)
					{
						free(ptr->keys);
						free(ptr->child);
						free(ptr->info);
						free(ptr);
						ptr=NULL;
						tr->root=y;
						for(int j=0; j<=y->n; j++)
						{
							y->child[j]->par=y;
						}
						tr->root->par=NULL;
					}
					//printf("tr->root value: %d\n", *tr->root->keys);
					int res=DelNode(ntr, key, rel);
					free(ntr);
					return res;	
				}
			}
		}
		ptr=ptr->child[ci];
	}
}

void MergeNode(Node* y, Node* z, Btree* tr)
{
	int f=0;
	int p=0;
	if(y->n==2*tr->t-1)
	{
		p=1;
		Node* ggpr=NULL;
		int indg=0;
		if(y->par)
		{
			ggpr=y->par;
			for(int j=0; j<2*tr->t; j++)
			{
				if(ggpr->child[j]==y)
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
			ggpr->child[0]=y;
			ggpr->par=NULL;
			tr->root=ggpr;
			y->par=ggpr;
		}
		y=Split(tr, ggpr, indg);
	}
	if(z->n==2*tr->t-1)
	{
		p=1;
		f=1;
		Node* ggpr=NULL;
		int indg=0;
		if(z->par)
		{
			ggpr=z->par;
			for(int j=0; j<2*tr->t; j++)
			{
				if(ggpr->child[j]==z)
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
			ggpr->child[0]=z;
			ggpr->par=NULL;
			tr->root=ggpr;
			z->par=ggpr;
		}
		Split(tr, ggpr, indg);
	}
	if((z->n + y->n > 2*tr->t-1) && !p)
	{
		y->keys[y->n]=z->keys[0];
		y->info[y->n].data=strdup(z->info[0].data);
		y->info[y->n].next=z->info[0].next;
		free(z->info[0].data);
		z->info[0].data=NULL;
		y->n=y->n+1;
		if(y->child[y->n-1])
		{
			MergeNode(y->child[y->n-1], z->child[0], tr);
		}
		else y->child[y->n-1]=z->child[0];
		for(int j=0; j<z->n; j++)
		{
			z->keys[j]=z->keys[j+1];
			z->info[j]=z->info[j+1];			
			z->child[j]=z->child[j+1];
		}
		z->n=z->n-1;
		//Afterd(z, tr);
		if(y->n==2*tr->t-1)
		{
			p=1;
			Node* ggpr=NULL;
			int indg=0;
			if(y->par)
			{
				ggpr=y->par;
				for(int j=0; j<2*tr->t; j++)
				{
					if(ggpr->child[j]==y)
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
				ggpr->child[0]=y;
				ggpr->par=NULL;
				tr->root=ggpr;
				y->par=ggpr;
			}
			y=Split(tr, ggpr, indg);
		}
	}

	//moving all keys and children from z node to y node
	for(int j=0; j<z->n; j++)
	{
		y->keys[y->n]=z->keys[j];
		y->info[y->n]=z->info[j];
		y->n=y->n+1;
		if(y->child[y->n-1])
		{
			if(y->child[y->n-1]!=z->child[j] && y->child[y->n-1]->n>0 && y->child[y->n-1]->n<=3) MergeNode(y->child[y->n-1], z->child[j], tr);
			else y->child[y->n-1]=z->child[j];
		}
		else y->child[y->n-1]=z->child[j];
	}
	y->child[y->n]=z->child[z->n];
	z->n=0;
	for(int j=0; j<=y->n; j++)
	{
		if(y->child[j]) y->child[j]->par=y;
	}
	//if(z!=tr->root)
	//{
	Node* zpar=z->par;
	free(z->keys);
	free(z->info);
	free(z->child);
	
	for(int i=0; i<zpar->n; i++)
	{
		if(zpar->child[i]==z)
		{
			zpar->child[i]=NULL;
		}
	}
	free(z);
	z=NULL;
	//}
	//removing z
	for(int i=0; i<2*tr->t-1; i++)
	{
		Item* gr=z->info+i;
		free(gr->data);
		gr->data=NULL;
	}
}
*/

int isLeaf(Node* ptr)
{
	int f=1;
	//if(!ptr->child) return 1;
	for(int i=0; i<=ptr->n; i++)
	{
		if(ptr->child[i])
		{
			f=0;
			break;
		}
	}
	return f;
}

int InfoDel(Node* ptr, int pos, int key, int rel)
{
	if(rel==1)
	{
		Item* sec=ptr->info[pos].next;
		free(ptr->info[pos].data);
		ptr->info[pos].data=ptr->info[pos].next->data;
		ptr->info[pos].next=ptr->info[pos].next->next;
		sec->next=NULL;
		free(sec);
		sec=NULL;
		return ERR_OK;
	}
	Item* gr=ptr->info+pos;
	Item* prev=ptr->info+pos;
	int k=0;
	while(gr)
	{
		k++;
		if(k==rel)
		{
			free(gr->data);
			gr->data=NULL;
			prev->next=gr->next;
			gr->next=NULL;
			free(gr);
			gr=NULL;
			return ERR_OK;
		}
		prev=gr;
		gr=gr->next;
	}
	return ERR_NF;
}

Node* find_right(Node* root)
{
	Node* pk=root;
	while(pk->child[pk->n]) pk=pk->child[pk->n];
	return pk;
}

Node* find_left(Node* root)
{
	Node* pk=root;
	while(pk->child[0]) pk=pk->child[0];
	return pk;
}

int DelNode(Btree* tr, int key, int rel)
{
	Node* ptr=tr->root;
	if(ptr)
	{
		//Looking for a key in this node
		int pos=-1;
		int ci=ptr->n;
		for(int i=0; i<ptr->n; i++)
		{
			if(ptr->keys[i]==key)
			{
				pos=i;
				break;
			}
			else if(ptr->keys[i]>key)
			{
				ci=i;
				break;
			}
		}

		//if it is a leaf and there is a key than simply deleting it.
		if(pos!=-1 && isLeaf(ptr))
		{
			//if there are several versions of a key
			if(ptr->info[pos].next) return InfoDel(ptr, pos, key, rel);
			else
			{
				//if incorrect release was entered
				if(rel!=1) return ERR_IR;
				else
				{
					//simple deletion of a single key from a leaf
					free(ptr->info[pos].data);
					ptr->info[pos].data=NULL;
					for(int i=pos; i<ptr->n-1; i++)
					{
						ptr->keys[i]=ptr->keys[i+1];
						ptr->info[i]=ptr->info[i+1];
					}
					ptr->n=ptr->n-1;
					return ERR_OK;
				}
			}
		}
		//if ptr is not a leaf
		else if(pos!=-1)
		{
			//deletion in case of multiple versions
			if(ptr->info->next) return InfoDel(ptr, pos, key, rel);
			else if(rel!=1) return ERR_IR;
			else
			{
				//left subtree case
				if(ptr->child[pos])
				{
					if(ptr->child[pos]->n>=tr->t)
					{
						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=ptr->child[pos];
						
						//Finding predseccor
						Node* pk=find_right(ntr->root);

						int ks=pk->keys[pk->n-1];
						char* is=strdup(pk->info[pk->n-1].data);
						DelNode(ntr, ks, 1);
						ptr->keys[pos]=ks;
						free(ptr->info[pos].data);
						ptr->info[pos].data=is;
						free(ntr);
						return ERR_OK;	
					//	Item* is=(Item*)malloc(sizeof(Item));
					//	Item* gr=pk->info+pk->n-1;
					//	while(gr)
					//	{
					//		is->data=strdup(gr->data);
					//		if(gr->next) is->next=(Item*)malloc(sizeof(Item));
					//		else is->next=NULL;
					//		gr=gr->next;
					//	}
					//	int r=DelNode(ntr, ks, 1);
					//	while(r==ERR_OK) r=DelNode(ntr, ks, 1);
					//	ptr->keys[pos]=ks;
					//	free(ptr->info[pos].data);
					//	ptr->info[pos]=*is;
					//	free(ntr);
					//	return ERR_OK;
					}
				}
				//right subtree case
				if(ptr->child[pos+1])
				{

					if(ptr->child[pos+1]->n>=tr->t)
					{
						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=ptr->child[pos+1];
						
						//Finding sucseccor
						Node* pk=find_left(ntr->root);

						int ks=pk->keys[0];
						char* is=strdup(pk->info[0].data);
						DelNode(ntr, ks, 1);
						ptr->keys[pos]=ks;
						free(ptr->info[pos].data);
						ptr->info[pos].data=is;
						free(ntr);
						return ERR_OK;
					}
				}
				//merging case
				if(ptr->child[pos] && ptr->child[pos+1])
				{
					Node* y=ptr->child[pos];
					Node* z=ptr->child[pos+1];
					
					//inserting key from ptr and key from z into y
					y->keys[y->n]=ptr->keys[pos];
					y->info[y->n].data=strdup(ptr->info[pos].data);
					y->n=y->n+1;

					y->child[y->n]=z->child[0];
					if(y->child[y->n]) y->child[y->n]->par=y;
					y->keys[y->n]=z->keys[0];
					y->info[y->n].data=strdup(z->info[0].data);
					y->info[y->n].next=z->info[0].next;
					y->n=y->n+1;

					y->child[y->n]=z->child[1];
					if(y->child[y->n]) y->child[y->n]->par=y;
					
					free(ptr->info[pos].data);
					free(z->info[0].data);
					
					//deleting key
					for(int i=pos; i<ptr->n-1; i++)
					{
						ptr->keys[i]=ptr->keys[i+1];
						ptr->info[i]=ptr->info[i+1];
					}
					free(z->keys);
					free(z->info);
					free(z->child);
					z->keys=NULL;
					z->info=NULL;
					z->child=NULL;
					free(z);
					z=NULL;

					//deleting z pointer
					for(int i=pos+1; i<ptr->n; i++)
					{
						ptr->child[i]=ptr->child[i+1];
					}
					ptr->child[ptr->n]=NULL;
					ptr->n=ptr->n-1;
					if(ptr->n==0)
					{
						Node* parent=ptr->par;
						if(!parent)
						{
							tr->root=y;
							y->par=NULL;
						}
						else 
						{
							y->par=parent;
							for(int j=0; j<=parent->n; j++)
							{
								if(parent->child[j]==ptr)
								{
									parent->child[j]=y;
									break;
								}
							}
						}
						free(ptr->keys);
						free(ptr->info);
						free(ptr->child);
						ptr->keys=NULL;
						ptr->info=NULL;
						ptr->child=NULL;
						free(ptr);
						ptr=NULL;
					}
					for(int i=0; i<=y->n; i++) if(y->child[i]) y->child[i]->par=y;
					Btree* ntr=(Btree*)malloc(sizeof(Btree));
					ntr->t=tr->t;
					ntr->root=y;
					int res=DelNode(ntr, key, rel);
					free(ntr);
					return res;
				}
			}
		}
		//if key not in this node
		else
		{
			if(!ptr->child[ci]) return ERR_NF;
			//If next searching node is balanced, go there and recursively delete
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
				//if left-hand neighbour have >=t keys than we distribute keys into our searching key
				if(ci-1>=0)
				{
					if(ptr->child[ci-1])
					{
						if(ptr->child[ci-1]->n>=tr->t)
						{
							Node* y=ptr->child[ci-1];
							Node* z=ptr->child[ci];
							
						//	z->keys[1]=z->keys[0];
						//	z->info[1]=z->info[0];
						//	z->child[2]=z->child[1];
						//	z->child[1]=z->child[0];
						//	z->child[0]=NULL;
							
							Btree* ntr=(Btree*)malloc(sizeof(Btree));
							ntr->t=tr->t;
							ntr->root=z;
							Item* gr=ptr->info+ci-1;
							while(gr)
							{
								AddNode(ntr, ptr->keys[ci-1], gr->data);
								gr=gr->next;
							}
						//	z->keys[0]=ptr->keys[ci-1];
						//	z->info[0]=ptr->info[ci-1];
						//	z->n=z->n+1;

							Node* pk=find_right(y);

							int k_d=pk->keys[pk->n-1];
							char* i_d=strdup(pk->info[pk->n-1].data);
							ntr->root=y;
							DelNode(ntr, k_d, 1);
							ptr->keys[ci-1]=k_d;
							ptr->info[ci-1].data=i_d;

							ntr->root=z;
							int res=DelNode(ntr, key, rel);
							free(ntr);
							return res;
						}
					}
				}
				//symmetric situation with right-hand neighbour if it exists
				if(ci+1<=ptr->n)
				{
					if(ptr->child[ci+1])
					{
						if(ptr->child[ci+1]->n>=tr->t)
						{
							Node* z=ptr->child[ci];
							Node* y=ptr->child[ci+1];
	
						//	z->keys[1]=z->keys[0];
						//	z->info[1]=z->info[0];
						//	z->child[2]=z->child[1];
						//	z->child[1]=z->child[0];
						//	z->child[0]=NULL;


							Btree* ntr=(Btree*)malloc(sizeof(Btree));
							ntr->t=tr->t;
							ntr->root=z;
							Item* gr=ptr->info+ci;
							while(gr)
							{
								AddNode(ntr, ptr->keys[ci], gr->data);
								gr=gr->next;
							}
						//	z->keys[0]=ptr->keys[ci];
						//	z->info[0]=ptr->info[ci];
						//	z->n=z->n+1;

							Node* pk=find_left(y);

							int k_d=pk->keys[0];
							char* i_d=strdup(pk->info[0].data);
							ntr->root=y;
							DelNode(ntr, k_d, 1);
							ptr->keys[ci]=k_d;
							ptr->info[ci].data=i_d;

							ntr->root=z;
							int res=DelNode(ntr, key, rel);
							free(ntr);
							return res;
						}
					}
				}
				//if none of neighbours have >=t keys
				if(ci-1>=0)
				{
					Node* y=ptr->child[ci-1];
					Node* z=ptr->child[ci];
					
					if(y && z)
					{
					//	printf("y==NULL: %d\n", y==NULL);
					//	printf("ptr==NULL: %d\n", ptr==NULL);
						y->keys[1]=ptr->keys[ci-1];
						y->info[1]=ptr->info[ci-1];
						y->child[2]=z->child[0];
						if(y->child[2]) y->child[2]->par=y;
						y->keys[2]=z->keys[0];
						y->info[2]=z->info[0];
						y->child[3]=z->child[1];
						if(y->child[3]) y->child[3]->par=y;
						y->n=y->n+2;
						
						free(z->keys);
						free(z->info);
						free(z->child);
						z->keys=NULL;
						z->info=NULL;
						z->child=NULL;
						free(z);
						z=NULL;
						
						if(ptr->n==1)
						{
							Node* parent=ptr->par;

							if(!parent) 
							{
								tr->root=y;
								y->par=NULL;
							}
							else
							{
								y->par=parent;
								for(int j=0; j<=parent->n; j++)
								{
									if(parent->child[j]==ptr)
									{
										parent->child[j]=y;
										break;
									}
								}
							}
							free(ptr->keys);
							free(ptr->info);
							free(ptr->child);
							ptr->keys=NULL;
							ptr->info=NULL;
							ptr->child=NULL;
							free(ptr);
							ptr=NULL;
							//for(int i=0; i<=y->n; i++) if(y->child[i]) y->child[i]->par=y;
						}
						else
						{
							for(int i=ci-1; i<ptr->n-1; i++)
							{
								ptr->keys[i]=ptr->keys[i+1];
								ptr->info[i]=ptr->info[i+1];
							}
							for(int i=ci; i<ptr->n; i++) ptr->child[i]=ptr->child[i+1];
							ptr->child[ptr->n]=NULL;
							ptr->n=ptr->n-1;
						}
						for(int i=0; i<=y->n; i++) if(y->child[i]) y->child[i]->par=y;
						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=y;
						int res=DelNode(ntr, key, rel);
						free(ntr);
						return res;
					}
				}
				if(ci+1<=ptr->n)
				{
					Node* y=ptr->child[ci];
					Node* z=ptr->child[ci+1];
					
					if(y && z)
					{
					//	printf("y==NULL: %d\n", y==NULL);
					//	printf("ptr==NULL: %d\n", ptr==NULL);
						y->keys[1]=ptr->keys[ci];
						y->info[1]=ptr->info[ci];
						y->child[2]=z->child[0];
						if(y->child[2]) y->child[2]->par=y;
						y->keys[2]=z->keys[0];
						y->info[2]=z->info[0];
						y->child[3]=z->child[1];
						if(y->child[3]) y->child[3]->par=y;
						y->n=y->n+2;
						
						free(z->keys);
						free(z->info);
						free(z->child);
						z->keys=NULL;
						z->info=NULL;
						z->child=NULL;
						free(z);
						z=NULL;
						
						if(ptr->n==1)
						{
							Node* parent=ptr->par;

							if(!parent) 
							{
								tr->root=y;
								y->par=NULL;
							}
							else 
							{
								y->par=parent;
								for(int j=0; j<=parent->n; j++)
								{
									if(parent->child[j]==ptr)
									{
										parent->child[j]=y;
										break;
									}
								}
							}
							free(ptr->keys);
							free(ptr->info);
							free(ptr->child);
							ptr->keys=NULL;
							ptr->info=NULL;
							ptr->child=NULL;
							free(ptr);
							ptr=NULL;
						}
						else
						{
							for(int i=ci; i<ptr->n-1; i++)
							{
								ptr->keys[i]=ptr->keys[i+1];
								ptr->info[i]=ptr->info[i+1];
							}
							for(int i=ci+1; i<ptr->n; i++) ptr->child[i]=ptr->child[i+1];
							ptr->child[ptr->n]=NULL;
							ptr->n=ptr->n-1;
						}
						for(int i=0; i<=y->n; i++) if(y->child[i]) y->child[i]->par=y;
						Btree* ntr=(Btree*)malloc(sizeof(Btree));
						ntr->t=tr->t;
						ntr->root=y;
						int res=DelNode(ntr, key, rel);
						free(ntr);
						return res;
					}
				}
			}
		}
	}
	return ERR_NF;
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
			//printf("ptr->child==NULL: %d\n", ptr->child==NULL);
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

int fimport(Btree* tr, char* fn)
{
	FILE* fd=fopen(fn, "r");
	if(!fd) return -1203912;
	while(!feof(fd))
	{
		int key=-1;
		fscanf(fd, "%d\n", &key);
		if(key==-1) break;
		char* info=enterf(fd);
		AddNode(tr, key, info);
	}
	fclose(fd);
	return ERR_OK;
}

void viz(Btree* tr, const char* fn)
{
	static int r=0;
//	FILE* fd=NULL;
//	if(!tr->root->par) fd=fopen(fn, "w+");
	FILE* fd=fopen(fn, "a+");
	Node* ptr=tr->root;
	if(ptr)
	{
		fprintf(fd, "subgraph cluster_%d {\n", r);
		if(isLeaf(ptr))
		{
			for(int i=ptr->n-1; i>=0; i--)
			{
				fprintf(fd, "%d\n", ptr->keys[i]);
			}
		}
		else
		{
			for(int i=0; i<ptr->n; i++)
			{
				fprintf(fd, "%d\n", ptr->keys[i]);
			}
		}
		fprintf(fd, "}\n");
		for(int i=ptr->n-1; i>=0; i--)
		{
			//if(ptr->child[i] && isLeaf(ptr->child[i])) fprintf(fd, "%d->%d\n", ptr->keys[i], ptr->child[i]->keys[ptr->child[i]->n-1]);
			if(ptr->child[i]) fprintf(fd, "%d->%d\n", ptr->keys[i], ptr->child[i]->keys[0]);
			//if(ptr->child[i+1] && isLeaf(ptr->child[i+1])) fprintf(fd, "%d->%d\n", ptr->keys[i], ptr->child[i+1]->keys[ptr->child[i+1]->n-1]);
			if(ptr->child[i+1]) fprintf(fd, "%d->%d\n", ptr->keys[i], ptr->child[i+1]->keys[0]);
		}
		r++;
		for(int j=0; j<=ptr->n; j++)
		{
			Btree* ntr=(Btree*)malloc(sizeof(Btree));
			ntr->t=tr->t;
			ntr->root=ptr->child[j];
			viz(ntr, fn);
			free(ntr);
		}
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


