#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"
#include <conio.h>

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
			continue;
		}
		else if(n==0) scanf("%*c");
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

//int parsing
int GetIntf(FILE* fd)
{
	int in=0, data;
	in=fscanf(fd, "%d", &data);
	if(in==EOF) 
	{
		//printf("End of file reached.\n");
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

void TableWrite(Table* t, char* fn)
{
	FILE* fd=fopen(fn, "w");
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
}

void menue()
{
	int key=0, pos=1;
	system("cls");
	const char* options[]={
		"import table from file.\n",
		"output table.\n",
		"search by key.\n",
		"search by version\n",
		"add\n",
		"delete by key\n",
		"delete by version\n",
		"save changes\n"
	};
	while(key!=13)
	{
		system("cls");
		for(int i=1; i<9; i++)
		{
			arrow(i, pos);
			printf("%s", options[i-1]);
		}
		key=getch(0);
		if(key==80 && pos!=8) pos++;
		else if(key==72 && pos!=1) pos--;
	}
}

int arrow(int cur, int pos)
{
	if(cur==pos) printf(">>>");
}

/*
int console(int input)
{
	//0 - menue
	//1 - input
	//2 - output
	//3 - search by key
	//4 - search by version
	//5 - add
	//6 - del by key
	//7 - del by version
	//8 - save changes
}*/


