#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcs.h"
#include "Btree.h"
#include "time.h"
#include "timer.h"

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
	in=fscanf(fd, "%d\n", &data);
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
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}


int getLL(long long int* n)
{
	int input;
	do
	{
		input=scanf("%lld", n);
		if(input==EOF) return 1;
		else if(input==0)
		{
			printf("Error, wrong data. Try again\n");
			scanf("%*[^\n]"); 
			continue;
		}
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}

int getDb(double* n)
{
	int input;
	do
	{
		input=scanf("%lf", n);
		if(input==EOF) return 1;
		else if(input==0)
		{
			printf("Error, wrong data. Try again\n");
			scanf("%*[^\n]"); 
			continue;
		}
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}

int fcheck(char* fnd)
{
	if(!fnd) 
	{
		free(fnd);
		fnd=NULL;
		return CERR_EOF;
	}
	if(*fnd==0) 
	{
		free(fnd);
		fnd=NULL;
		fnd=enter();
	}
	return CERR_OK;
}

//MVC paradigm
//------------------------
//Main Controller function
int console(int p, Btree* tr)
{	
	int (*view[])(Btree*)={Importv, Traverv, Addv, Delv, Searchv, Maxv, Minv, Showv, Timingv}; //, Showv, Generv, Timingv
	return view[p](tr);	
}

//options
//0 - Import tree from text file
//1 - Traversing of tree
//2 - Add node to tree
//3 - Delete node from tree
//4 - Search node by key
//5 - Find Maximum
//6 - Find Minimum
//7 - Show Tree as a tree

//View functions 

int Importv(Btree* tr)
{
	//if(root->info) printf("Warning. if your tree is not empty then this function will nest new tree into current tree.\n");
	printf("Enter filename: ");
	char* fn=enter();
	if(fcheck(fn)==CERR_EOF) return CERR_EOF;

	fimport(tr, fn);
	//int result=fimport(root, fn);
//	if(result==ERR_EOF) 
//	{
//		free(fn);
//		return CERR_EOF;
//	}
	printf("Tree was successfully imported from %s\n", fn);
	free(fn);
	return EndView();
}

int Traverv(Btree* tr)
{
	printf("Enter a key where traversing starts or enter any negative value to traverse all nodes: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	//if(key<0) key=FULL_TREE;
	
	double t1, t2;
	t1=clock();
	int result=Traversing(tr, key);
	t2=clock();
	//if(result==ERR_EMPTY) printf("Error. Tree is empty.\n");
	printf("Excecution time %lf seconds.\n", (t2-t1)/CLOCKS_PER_SEC);
	return EndView();
}

int Addv(Btree* tr)
{
	printf("Enter key: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	printf("Enter data: ");
	char* data=enter();
	if(!data) return CERR_EOF;

	double t1, t2;
	t1=clock();
	int result=AddNode(tr, key, data);
	t2=clock();
//	if(result==ERR_DUPL)
//	{
//		free(data);
//		printf("Error. Node with this key has already been added to tree.\n");
//	}
	//printf("Node with key %d was successfully added to tree.\n", key);
	
	printf("Tree:\n");
	//Traversing(root, FULL_TREE);
	printf("Excecution time %lf seconds.\n", (t2-t1)/CLOCKS_PER_SEC);
	return EndView();
}

int Delv(Btree* tr)
{
	printf("Enter key: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	printf("Enter a number of element: ");
	int rel=0;
	input=getInt(&rel);
	if(input) return CERR_EOF;

	double t1, t2;
	t1=clock();
	int result=DelNode(tr, key, rel);
	t2=clock();
	if(result==ERR_NF) printf("Error. No such key in tree.\n");
	else if(result==ERR_IR) printf("Error. Incorrect number of element was entered.\n");
	else printf("Key %d after number %d was successfully deleted from tree.\n", key, rel);

	printf("Excecution time %lf seconds.\n", (t2-t1)/CLOCKS_PER_SEC);
	return EndView();
}

int Searchv(Btree* tr)
{
	printf("Enter key: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	
	double t1, t2;
	t1=clock();
	Item* result=Search(tr, key);
	t2=clock();
	if(!result) printf("Error. No node with such key in tree.\n");
	else
	{
		printf("key: %d | info: ", key);
		while(result)
		{
			printf("\"%s\", ", result->data);
			result=result->next;
		}
		//printf("key: %d | info: %s\n", result->key, result->info);
	}
	printf("Excecution time %lf seconds.\n", (t2-t1)/CLOCKS_PER_SEC);

	return EndView();
}

int Maxv(Btree* tr)
{
//	double t1, t2;
//	t1=clock();
//	Node* result=Max(root);
//	t2=clock();
//	if(!result) printf("Error. Tree is empty.\n");
//	else
//	{
//		printf("key: %d | info: %s\n", result->key, result->info);
//	}
//	printf("Excecution time %lf seconds.\n", (t2-t1)/CLOCKS_PER_SEC);

	return EndView();
}

int Minv(Btree* tr)
{
	printf("Enter key: ");
	int key=0;
	int input=getInt(&key);
	if(input) return CERR_EOF;
	double t1, t2;
	t1=clock();
	Node* result=Min(tr, key);
	t2=clock();
	int ind=0, f=0;
	for(int i=0; i<result->n; i++)
	{
		if(result->keys[i]>key) 
		{
			f=1;
			ind=i;
			break;
		}
	}
	if(!f)
	{
		printf("Error. Entered key is bigger than any key in tree.\n");
		return EndView();
	}
	printf("key: %d | info: ", result->keys[ind]);
	Item* gr=result->info+ind;
	while(gr)
	{
		printf("\"%s\", ", gr->data);
		gr=gr->next;
	}
	printf("\n");
//	for(int i=0; i<result->n; i++)
//	{
//		printf("key: %d | info: ", result->keys[i]);
//		Item* rp=result->info+i;
//		while(rp)
//		{
//			printf("\"%s\", ", rp->data);
//			rp=rp->next;
//		}
//		printf("\n");
//	}
//	if(!result) printf("Error. Tree is empty.\n");
//	else
//	{
//		printf("key: %d | info: %s\n", result->key, result->info);
//	}
	printf("Excecution time %lf seconds.\n", (t2-t1)/CLOCKS_PER_SEC);

	return EndView();
}


int Showv(Btree* tr)
{
	double t1, t2;
	t1=clock();
	Node* ptr=tr->root;
//	if(ptr)
//	{
//		printf("[ ");
//		for(int j=0; j<ptr->n; j++)
//		{
//			printf("%d ", ptr->keys[j]);
//		}
//		printf("] ");
//	}
//	else
//	{
//		printf("[ ");
//		for(int j=0; j<2*tr->t -1; j++) printf(" ");
//		printf("] ");
//	}
	printf("\n");
	show(tr, 0);
	t2=clock();
	//if(result==ERR_EMPTY) printf("Error. Tree is empty.\n");
	printf("Excecution time %lf seconds.\n", (t2-t1)/CLOCKS_PER_SEC);

	return EndView();
}
/*
int Generv(Node* root)
{
	printf("Enter amount of nodes in tree: ");
	int n=0;
	int input=getInt(&n);
	if(input) return CERR_EOF;
	printf("Now enter key generation limit: ");
	int limit=0;
	input=getInt(&limit);
	if(input) return CERR_EOF;
	printf("Lastly enter info size limit: ");
	int str_limit=0;
	input=getInt(&str_limit);
	if(input) return CERR_EOF;


	Node* start=Max(root);
	while(start!=NULL)
	{
		free(start->info);
		start->info=NULL;
		Node* prev=start->prev;
		if(start!=root) free(start);
		start=prev;
	}
	generate(root, n, limit, str_limit);

	return EndView();
}*/

int Timingv(Btree* tr)
{
	printf("Enter maximum amount of nodes in tree: ");
	long long n=0;
	int input=getLL(&n);
	if(input) return CERR_EOF;
	printf("Now enter key generation limit: ");
	long long limit=0;
	input=getLL(&limit);
	if(input) return CERR_EOF;
	printf("enter info size limit: ");
	long long str_limit=0;
	input=getLL(&str_limit);
	if(input) return CERR_EOF;
	printf("enter amount of iterations for every operation: ");
	int itr=0;
	input=getInt(&itr);
	if(input) return CERR_EOF;
	printf("Lastly, enter step of testing: ");
	double step=0;
	input=getDb(&step);
	if(input) return CERR_EOF;
	
	timing(n, limit, str_limit, itr, step);
	printf("Timing results were written in directory that was passed to a function.\n");
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
