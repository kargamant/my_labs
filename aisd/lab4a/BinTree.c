#include <stdio.h>
#include <stdlib.h>

void Traversing(Node* tr, int key)
{
	//going to write search and then traversing
}

Node* FindPrev(Node* root, Node* x)
{
	if(x==NULL) return NULL;
	if(x->left!=NULL)
	{
		return Max(x->left);
	}
	else
	{
		Node* par=NULL;
		Node* ptr=root;
		//path to x - all parents that were visited
		Node** path=(Node**)malloc(sizeof(Node*));
		Node** pth=path;
		while(ptr!=x)
		{
			if(ptr->key<x->key)
			{
				par=ptr;
				ptr=ptr->right;	
			}
			else
			{
				par=ptr;
				ptr=ptr->left;
			}
			*pth=par;
			path=(Node**)realloc(path, (pth-path+2)*sizeof(Node*));
			++pth;
		}
		--pth;
		while(*pth!=*(pth-1)->right)
		{
			par=*(pth-1);
			--pth;
		}
		par=*(pth-1);
		free(path);
		path=NULL;
		return par;
	}
}
