#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "TableLib/Table.h"
#include "funcs.h"
#include "HashLib/Hash.h"
#include <ncurses.h>

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
		else if(*n<0)
		{
			printf("Error. Value must be positive. Try again.\n");
			continue;
		}
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}

//function for saving table at .txt file
/*
int TableWrite(Table* t, char* fn)
{
	FILE* fd=fopen(fn, "w");
	if(!fd) return ERR_FIL;
	fprintf(fd, "%d\n", t->msize);
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize)
	{
		fprintf(fd, "%d ", ptr->key);
		Node* gr=ptr->node;
		while(gr)
		{
			fprintf(fd, "%s ", gr->item->data);
			gr=gr->next;
		}
		fprintf(fd, "\n");
		++ptr;
	}
	fclose(fd);
	return ERR_OK;
}*/

//MVC paradigm
//------------------------
//Main Controller function
int console(int p, Table* t)
{	
	int (*view[])(Table*)={Newv, Outv, Searchkv, Searchvv, Addv, Delkv, Delvv};
	return view[p](t);	
}

//options
//0 - create table from console
//1 - output
//2 - search by key
//3 - search by version
//4 - add
//5 - del by key
//6 - del by version

//View functions 

int Newv(Table* t)
{
	printf("Enter msize of your table: ");
	int msize=0;
	int in=getInt(&msize);
	if(in) return CERR_EOF;

	if(!isPrime(msize))
	{
		printf("Warning. Not a prime msize was entered.\nBut the hash function is working only with prime msizes.\n");
		while(!isPrime(msize))
		{
			msize++;
		}
		printf("Your table was reinitialized with closest prime msize to that you have entered. New msize=%d\n", msize);
	}
	New(msize, t);
	printf("Table was succesfully initialized with msize=%d\n", msize);

	return EndView();
}

int Outv(Table* t)
{
	output(t);
	return EndView();
}

int Searchkv(Table* t)
{
	int key=0;
	printf("Enter a key: ");
	int in=getInt(&key);
	if(in) return CERR_EOF;

	Table* result=SearchByKey(t, key);
	if(result)
	{
		output(result);	
		erased(result);	
		free(result);
		//proper freeing
	}
	else printf("No KeySpace was found.\n");	
			
	return EndView();
}

int Searchvv(Table* t)
{
	int key=0, rel=0;
	printf("Enter a key: ");
	int in=getInt(&key);
	if(in) return CERR_EOF;

	printf("Now enter a version: ");
	in=getInt(&rel);
	if(in) return CERR_EOF;

	Table* res=SearchByVersion(t, key, rel);
	if(res)
	{
		output(res);	
		erased(res);
		free(res);
		//proper freeing
	}
	else printf("No element found after this key and rel.\n");
	

	return EndView();
}

int Addv(Table* t)
{
	int key=0;
	printf("Enter a key: ");
	int in=getInt(&key);
	if(in) return CERR_EOF;

	printf("Enter data: ");
	char* data=enter();
	if(!data) return CERR_EOF;

	in=add(t, key, data);
	if(in==ERR_FULL) 
	{
		free(data);
		printf("Error. Table is full.\n");
	}
	output(t);
	//free(data);
	//proper freeing
	return EndView();
}

int Delkv(Table* t)
{
	int key=0;
	printf("Enter a key: ");
	int in=getInt(&key);
	if(in) return CERR_EOF;

	in=DelByKey(t, key);
	if(in==ERR_NF) printf("Error. No such key in table.\n");
	else output(t);

	return EndView();
}

int Delvv(Table* t)
{
	int key=0, rel=0;
	printf("Enter a key: ");
	int in=getInt(&key);
	if(in) return CERR_EOF;

	printf("Now enter a version: ");
	in=getInt(&rel);
	if(in) return CERR_EOF;

	in=DelByVersion(t, key, rel);
	if(in==ERR_NF) printf("Error. No such key or version in table.\n");
	else output(t);

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
