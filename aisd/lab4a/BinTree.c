#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"
#include <string.h>

int Traversing(Node* root, int key)
{
	Node* ptr=NULL;
	if(root->info==NULL) return ERR_EMPTY;
	if(key==FULL_TREE) ptr=Max(root);
	else ptr=Search(root, key);
	if(ptr==NULL) return ERR_NF;
	while(ptr!=NULL)
	{
		printf("key: %d | info: %s\n", ptr->key, ptr->info);
		ptr=ptr->prev;
	}
	return ERR_OK;
}

Node* Max(Node* root)
{
	Node* ptr=root;
	if(!ptr) return NULL;
	while(ptr->right!=NULL) ptr=ptr->right;
	return ptr;
}

Node* Min(Node* root)
{
	Node* ptr=root;
	if(!ptr) return NULL;
	while(ptr->left!=NULL) ptr=ptr->left;
	return ptr;
}

Node* Search(Node* root, int key)
{
	Node* ptr=root;
	while(ptr->key!=key)
	{
		if(ptr->key<key) ptr=ptr->right;
		else ptr=ptr->left;
		if(ptr==NULL) return NULL;
	}
	return ptr;
}

int AddNode(Node* root, int key, char* info)
{
	Node* x=(Node*)malloc(sizeof(Node));
	x->key=key;
	x->info=info;
	x->right=NULL;
	x->left=NULL;
	x->prev=NULL;
	if(root->info==NULL) 
	{
		root->key=x->key;
		root->info=x->info;
		root->left=NULL;
		root->right=NULL;
		root->prev=root->left;
		free(x);
		x=NULL;
		return ERR_OK;
	}
	Node* ptr=root;
	Node* par=NULL;
	Node* prev=NULL;
	Node* next=NULL;
	while(ptr!=NULL)
	{
		par=ptr;
		if(ptr->key<key) 
		{
			ptr=ptr->right;
			prev=par;
		}
		else if(ptr->key>key)
		{
			ptr=ptr->left;
			next=par;
		}
		else
		{
			erased(x);
			return ERR_DUPL;
		}
	}
	if(par->key>x->key) par->left=x;
	else par->right=x;
	x->prev=prev;
	if(next) next->prev=x;
	return ERR_OK;
}

int DelNode(Node* root, int key)
{
	Node* ptr=root;
	if(ptr->info==NULL) return ERR_EMPTY;
	Node* par=NULL;
	Node* prev=NULL;
	Node* next=NULL;
	while(ptr->key!=key)
	{
		par=ptr;
		if(ptr->key<key) 
		{
			ptr=ptr->right;
			prev=par;
		}
		else
		{
			ptr=ptr->left;
			next=par;
		}
		if(ptr==NULL) return ERR_NF;
	}
	if(ptr->right==NULL || ptr->left==NULL)
	{
		if(ptr->right==NULL)
		{
			if(par->right==ptr) par->right=ptr->left;
			else par->left=ptr->left;
			if(ptr->left!=NULL)
			{
				prev=Max(ptr->left);
			}
			if(next) next->prev=prev;
			ptr->right=NULL;
			ptr->left=NULL;
			ptr->prev=NULL;
			free(ptr->info);
			free(ptr);
		}
		else
		{
			if(par->right==ptr) par->right=ptr->right;
			else par->left=ptr->right;
			next=Min(ptr->right);	
			next->prev=prev;
			ptr->right=NULL;
			ptr->left=NULL;
			ptr->prev=NULL;
			free(ptr->info);
			free(ptr);
		}
	}
	else
	{
		prev=Max(ptr->left);

		Node* next_par=ptr;
		Node* rp=ptr->right;
		while(rp->left!=NULL)
		{
			next_par=rp;
			rp=rp->left;
		}
		//next=rp;
		if(next_par==ptr) next_par->right=rp->right;
		else next_par->left=NULL;

		Node* left=ptr->left;
		Node* right=ptr->right;
		rp->right=right;
		rp->left=left;
		if(par->right==ptr) par->right=rp;
		else par->left=rp;
		rp->prev=prev;
		free(ptr->info);
		free(ptr);
	}
	return ERR_OK;
}

void erased(Node* root)
{
	Node* start=Max(root);
	while(start!=NULL)
	{
		free(start->info);
		Node* prev=start->prev;
		free(start);
		start=prev;
	}
}

int fimport(Node* root, char* fn)
{
	FILE* fd=fopen(fn, "r");
	fseek(fd, 0, SEEK_END);
	int end_of_fd=ftell(fd);
	fseek(fd, 0, SEEK_SET);
	while(ftell(fd)!=end_of_fd)
	{
		int key=GetIntf(fd);
		if(key==-1) 
		{
			erased(root);
			return ERR_EOF;
		}
		char* info=enterf(fd);
		if(!info) 
		{
			erased(root);
			return ERR_EOF;
		}
		AddNode(root, key, info);
	}
	fclose(fd);
	return ERR_OK;
}

int show(Node* root)
{
	if(root->info==NULL) return ERR_EMPTY;	
	
	//Generating tree layout
	Node** nodes=(Node**)malloc(sizeof(Node*));
	*nodes=root;
	int p=1;
	int k=0;
	int n=1;
	while(p!=k)
	{
		p*=2;
		k=0;
		nodes=(Node**)realloc(nodes, (n+p)*sizeof(Node*));
		int jsum=0;
		for(int j=n; j<n+p; j+=2)
		{
			if(nodes[j-p/2-jsum]) nodes[j]=nodes[j-p/2-jsum]->left;
			else nodes[j]=NULL;
			if(nodes[j-p/2-jsum]) nodes[j+1]=nodes[j-p/2-jsum]->right;
			else nodes[j+1]=NULL;
			if(!nodes[j]) k++;
			if(!nodes[j+1]) k++;
			jsum++;
		}
		n+=p;
		if(k==p) 
		{
			n-=p;
			break;
		}
	}
	int spaces=n;
	int branches=(n+1)/2;
	int sum_indent=0;
	while(branches!=0)
	{
		sum_indent+=branches;
		branches/=2;
	}
	branches=(n+1)/2;
	p=1;

	for(int i=0; i<n; i+=p/2)
	{	
		for(int j=i; j<i+p; j++) 
		{
			if(j==i) for(int j=0; j<sum_indent-branches; j++) printf(" ");
			for(int j=0; j<branches; j++) printf("-");
			if(nodes[j]) printf("%d", nodes[j]->key);
			for(int j=0; j<branches; j++) printf("-");
			for(int j=0; j<spaces; j++) printf(" ");

		}
		printf("\n");
		spaces=spaces/2;
		sum_indent-=branches;
		branches/=2;
		p*=2;
	}
	free(nodes);
	return ERR_OK;
}

void out_node(Node* x, int spaces)
{
	
	for(int i=0; i<spaces; i++) printf(" ");
	if(x) printf("%d", x->key);
	else 
	{
		for(int i=0; i<spaces+1; i++) printf(" ");
		//printf("db1\n");
	}
}
