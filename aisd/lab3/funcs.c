#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"
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
Table* TableWrite(Table* t, char* fn)
{
	FILE* fd=fopen(fn, "w");
	if(!fd) return NULL;
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
	return t;
}
/*
//interactive arrow menue
int menue()
{
	int key=0, pos=1;
	system("clear");
	const char* options[]={
		"import table from file.\n",
		"output table.\n",
		"search by key.\n",
		"search by version\n",
		"add\n",
		"delete by key\n",
		"delete by version\n",
		"save changes\n",
		"quit\n"
	};
	
	initscr();
	while(key!=10)
	{
		clear();
		for(int i=1; i<10; i++)
		{
			arrow(i, pos);
			printw("%s", options[i-1]);
		}
		key=getch();
		if(key==115) 
		{
			pos=(pos+1)%10;
			if(!pos) pos=1;
		}
		else if(key==119) 
		{
			pos=(10+(pos-1)%10)%10;
			if(!pos) pos=9;
		}
	}
	endwin();
	return pos;
}

//displaying arrow in a specific position
int arrow(int cur, int pos)
{
	if(cur==pos)
	{
		printw(">>>");
	}
}*/

//MVC paradigm
//------------------------
//Main Controller function
int console(int p, Table* t)
{	
	int (*view[])(Table*)={NULL, Inputv, Outv, Searchkv, Searchvv, Addv, Delkv, Delvv, Savev};
	view[p](t);	
}


//view of file import function
/*
Table* fimport()
{
	do
	{
		printf("Enter FileName with table: ");
		char* FileName=enter();
		if(!FileName) return NULL;
		FILE *fd=fopen(FileName, "r");
		if(!fd)
		{
			printf("File does not exist or wrong FileName. Try again.\n");
			continue;
		}
		Table* t=input(fd);	
		if(!t)
		{
			printf("Error. Got wrong data while parsing. Try again.\n");
			continue;
		}
		fclose(fd);
		free(FileName);
		FileName=NULL;
		return t;
	}while(1);
}*/

//1 - input
//2 - output
//3 - search by key
//4 - search by version
//5 - add
//6 - del by key
//7 - del by version
//8 - save changes

//View functions 
/*int fimport(Table* t)
{

}*/

int Inputv(Table* t)
{
	if(t) erased(t);
	do
	{
		printf("Enter FileName with table: ");
		char* FileName=enter();
		if(!FileName) 
		{
			free(FileName);
			FileName=NULL;
			return CERR_EOF;
		}
		if(*FileName==0) 
		{
			free(FileName);
			FileName=enter();
		}
		
		FILE *fd=fopen(FileName, "r");
		if(!fd)
		{
			printf("File does not exist or wrong FileName. Try again.\n");
			free(FileName);
			FileName=NULL;
			continue;
		}	
			
		if(input(fd, &t)==ERR_WRD)
		{
			printf("Error. Got wrong data while parsing. Try again.\n");
			fclose(fd);
			free(FileName);
			FileName=NULL;
			continue;
		}
		fclose(fd);
		free(FileName);
		FileName=NULL;
		return EndView();
	}while(1);
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

	KeySpace* result=SearchByKey(t, key);
	if(result) outputks(result);
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

	Node* res=SearchByVersion(t, key, rel);
	if(res)
	{
		outputnd(res);
		printf("\n");
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
	if(in==ERR_FULL) printf("Error. Table is full.\n");
	output(t);
	free(data);
	return EndView();
}

int Delkv(Table* t)
{
	int key=0;
	printf("Enter a key: ");
	int in=getInt(&key);
	if(in) return CERR_EOF;

	in=DelByKey(t, key);
	if(in==ERR_NO_FOUND) printf("Error. No such key in table.\n");
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
	if(in==ERR_NO_FOUND) printf("Error. No such key or version in table.\n");
	else output(t);

	return EndView();
}

int Savev(Table* t)
{
	printf("Enter a filename where table will be saved: ");
	char* fn=enter();
	if(!fn) return CERR_EOF;
			
	Table* p=TableWrite(t, fn);
	if(!p) printf("Error. Wrong filename.\n");
	else printf("Modified table was saved at %s\n", fn);
	free(fn);
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
