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
			//erased(x);
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
	/*
		100
	    50	      120
		   110   140
	 */
	if(ptr==root)
	{
		Node* next_par=ptr;
		Node* rp=ptr->right;
		if(rp) 
		{	while(rp->left!=NULL)
			{
				next_par=rp; //120
				rp=rp->left; //110
			}

			if(next_par==ptr) next_par->right=rp->right;
			else next_par->left=rp->right;

			ptr->key=rp->key;
			free(ptr->info);
			ptr->info=rp->info;
			if(rp->right==NULL) next_par->prev=ptr;
			else Min(rp->right)->prev=ptr;
			free(rp);
		}	
		else
		{
			if(!ptr->left)
			{
				free(ptr->info);
				ptr->info=NULL;
				return ERR_OK;
			}
			free(ptr->info);
			Node* new_root=ptr->left;
			if(!new_root->right) 
			{
				ptr->left=new_root->left;
				ptr->right=NULL;
				ptr->key=new_root->key;
				ptr->info=new_root->info;
				ptr->prev=new_root->prev;
				free(new_root);
			}
			else
			{
				next=new_root->right;
				next_par=new_root;
				while(next->right!=NULL)
				{
					next_par=next;
					next=next->right;
				}
				next_par->right=next->left;
				ptr->key=next->key;
				ptr->info=next->info;
				ptr->prev=next->prev;
				free(next);
			}
		}
				
	}
	else if(ptr->right==NULL || ptr->left==NULL)
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
		else next_par->left=rp->right;

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
		int result=AddNode(root, key, info);
		if(result!=ERR_OK) free(info);
	}
	fclose(fd);
	return ERR_OK;
}

int show(Node* root)
{
	if(!root) return ERR_EMPTY;
	if(root->info==NULL) return ERR_EMPTY;
	assymetric_trave(root, 0);
	return ERR_OK;
}

void assymetric_trave(Node* root, int h)
{
	int hc=h;
	if(!root) printf("\n");
	else
	{
		assymetric_trave(root->right, h+1);
		for(int i=0; i<hc; i++) printf("    ");
		printf("%d\n", root->key);
		assymetric_trave(root->left, h+1);
	}
}
