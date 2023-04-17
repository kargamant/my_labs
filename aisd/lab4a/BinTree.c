#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"

void Traversing(Node* root, int key)
{
	Node* ptr=NULL;
	if(key==FULL_TREE) ptr=Max(root);
	else ptr=Search(root, key);
	while(ptr!=NULL)
	{
		printf("key: %d | info: %s\n", ptr->key, ptr->info);
		ptr=ptr->prev;
	}
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
	if(root==NULL) 
	{
		root=x;
		root->prev=root->left;
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
			free(x);
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
	if(ptr->right==NULL && ptr->left==NULL)
	{
		if(par->right==ptr) par->right=NULL;
		else par->left=NULL;
		erased(ptr);
		free(ptr);
		next->prev=prev;
		return ERR_OK;
	}
	else if(ptr->right!=NULL && ptr->left==NULL)
	{
		if(par->right==ptr)
		{
			par->right=ptr->right;
		}
		else par->left=ptr->right;
		erased(ptr);
		free(ptr);
		next=Min(ptr->right);
		next->prev=prev;
		return ERR_OK;
	}
	else if(ptr->left!=NULL && ptr->right==NULL)
	{
		if(par->right==ptr)
		{
			par->right=ptr->left;
		}
		else par->left=ptr->left;
		erased(ptr);
		free(ptr);
		prev=Max(ptr->left);
		next->prev=prev;
		return ERR_OK;
	}
	else
	{
		Node* y=ptr;
		Node* right=ptr->right;
		while(right->left!=NULL) 
		{
			par=right;
			right=right->left;		
		}
		Node* next_par=par;
		Node* left=ptr->left;
		while(left->right!=NULL)
		{
			par=left;
			left=left->right;
		}
		Node* prev_par=par;
		next_par->left=NULL;
		y->key=right->key;
		y->info=right->info;
		y->prev=left;
		return ERR_OK;
	}
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
	FILE* fd=fopen(fn, "r+");
	while(!feof(fd))
	{
		Node* x=(Node*)malloc(sizeof(Node));
		fscanf(fd, "%d", &(x->key));
		x->info=enterf(fd);
		AddNode(root, x);
	}
	fclose(fd);
}
