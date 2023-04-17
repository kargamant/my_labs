#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcs.h"
#include "BinTree.h"

//string parsing from lab2
char* enter()
{
	char buf[81]={0};
	char* ptr=(char*)malloc(1);
	*ptr='\0';
	int n, len=0;
	do
	{
		n=scanf("%80[^\n]", buf);
		if(n<0) 
		{
			free(ptr);
			ptr=NULL;
			break;
		}
		else if(n==0) 
		{
			scanf("%*c");
		}
		else 
		{
			len+=strlen(buf);
			ptr=(char*)realloc(ptr, len+1);
			strcat(ptr, buf);

		}
	}while(n>0);
	return ptr;
}

//file parsing version
char* enterf(FILE* fd)
{
	char buf[81]={0};
	char* ptr=(char*)malloc(1);
	*ptr='\0';
	int n, len=0;
	do
	{
		n=fscanf(fd, "%80[^\n]", buf);
		if(n<0) 
		{
			free(ptr);
			ptr=NULL;
			continue;
		}
		else if(n==0) fscanf(fd, "%*c");
		else 
		{
			len+=strlen(buf);
			ptr=(char*)realloc(ptr, len+1);
			strcat(ptr, buf);

		}
	}while(n>0);
	return ptr;
}

//int parsing from file
int GetIntf(FILE* fd)
{
	int in=0, data;
	in=fscanf(fd, "%d", &data);
	if(in==EOF) 
	{
		return -1;
	}
	else if(in==0)
	{
		printf("Error. Wrong data.\n");
		return -1;
	}
	else if(data<0)
	{
		printf("Error. Key or msize must be positive.\n");
		return -1;
	}
	else return data;
}

//Correct input of integer from lab1
int getInt(int* n)
{
	int input;
	do
	{
		input=scanf("%d", n);
		if(input==EOF) return 1;
		else if(input==0)
		{
			printf("Error, wrong data. Try again\n");
			scanf("%*[^\n]"); 
			continue;
		}
		/*else if(*n<0)
		{
			printf("Error. Value must be positive. Try again.\n");
			continue;
		}*/
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}

//MVC paradigm
//------------------------
//Main Controller function
int console(int p, Node* root)
{	
	int (*view[])(Node*)={Importv, Traverv, Addv, Delv, Searchv, Maxv, Minv};
	return view[p](root);	
}

//options
//0 - Import tree from text file
//1 - Traversing of tree
//2 - Add node to tree
//3 - Delete node from tree
//4 - Search node by key
//5 - Find Maximum
//6 - Find Minimum

//View functions 

int Importv(Node* root)
{
	printf("Enter filename: ");
	char* fn=enter();
	if(!fn) return CERR_EOF;
	
	int result=fimport(root, fn);
	if(result==ERR_EOF) 
	{
		free(fn);
		return CERR_EOF;
	}
	printf("Tree was successfully imported from %s\n", fn);
	free(fn);
	return EndView();
}

int Traverv(Node* root)
{
	printf("Enter a key where traversing starts or enter any negative value to traverse all nodes: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	if(key<0) key=FULL_TREE;

	int result=Traversing(root, key);
	if(result==ERR_NF) printf("Error. No match with start key in tree.\n");
	else if(result==ERR_EMPTY) printf("Error. Tree is empty.\n");
	return EndView();
}

int Addv(Node* root)
{
	printf("Enter key: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	printf("Enter data: ");
	char* data=enter();
	if(!data) return CERR_EOF;

	int result=AddNode(root, key, data);
	if(result==ERR_DUPL)
	{
		printf("Error. Node with this key has already been added to tree.\n");
	}
	else printf("Node with key %d was successfully added to tree.\n", key);
	
	printf("Tree:\n");
	Traversing(root, FULL_TREE);
	return EndView();
}

int Delv(Node* root)
{
	printf("Enter key: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	
	int result=DelNode(root, key);
	if(result==ERR_NF) printf("Error. No node with this key in tree.\n");
	else printf("Node with key %d was successfully deleted.", key);

	return EndView();
}

int Searchv(Node* root)
{
	printf("Enter key: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	
	Node* result=Search(root, key);
	if(!result) printf("Error. No node with such key in tree.\n");
	else
	{
		printf("key: %d | info: %s\n", result->key, result->info);
	}

	return EndView();
}

int Maxv(Node* root)
{
	Node* result=Max(root);
	if(!result) printf("Error. Tree is empty.\n");
	else
	{
		printf("key: %d | info: %s\n", result->key, result->info);
	}

	return EndView();
}

int Minv(Node* root)
{
	Node* result=Min(root);
	if(!result) printf("Error. Tree is empty.\n");
	else
	{
		printf("key: %d | info: %s\n", result->key, result->info);
	}

	return EndView();
}

//Function that ends a view function
int EndView()
{
	int i=0;
	printf("Input any value to continue or EOF to stop.\n");
	int cont=scanf("%d", &i);
	scanf("%*[^\n]");
	if(cont==EOF) return CERR_EOF;
	return CERR_OK;
}
