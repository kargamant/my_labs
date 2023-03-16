#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"
#include <ncurses.h>

//string parsing from lab2
char* enter()
{
	//scanf("%*[^\n]");
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

//int parsing from file
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
		"save changes\n"
	};
	
	initscr();
	while(key!=10)
	{
		clear();
		refresh();
		//system("clear");	
		for(int i=1; i<9; i++)
		{
			arrow(i, pos);
			printw("%s", options[i-1]);
			refresh();
		}
		key=getch();
		//printw("%d", key);
		//printw("\n%d\n", key);
		if(key==115 && pos!=8) pos++;
		else if(key==119 && pos!=1) pos--;
	}
	endwin();
	return pos;
}

int arrow(int cur, int pos)
{
	if(cur==pos)
	{
		printw(">>>");
		refresh();
	}
}


int console(int input, Table* t)
{
	//1 - input
	//2 - output
	//3 - search by key
	//4 - search by version
	//5 - add
	//6 - del by key
	//7 - del by version
	//8 - save changes
	int key=0, rel=0, in=0, cont=0, i=0;
	switch(input)
	{
		case 1:
			erased(t);
		//	if(t) 
		//	{
		//		output(t);
		//		erased(t);
		//	}
			t=fimport();
			if(t) output(t);
		
			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;
		case 2:
			output(t);
			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;
		case 3:
			printf("Enter a key: ");
			in=getInt(&key);
			if(in) return CERR_EOF;

			KeySpace* result=SearchByKey(t, key);
			if(result) outputks(result);
			else printf("No KeySpace was found.\n");
			
			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;
		case 4:
			printf("Enter a key: ");
			in=getInt(&key);
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

			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;
		case 5:
			printf("Enter a key: ");
			in=getInt(&key);
			if(in) return CERR_EOF;
			printf("Enter data: ");
			char* data=enter();
			if(!data) return CERR_EOF;

			i=add(t, key, data);
			if(i==ERR_FULL) printf("Error. Table is full.\n");
			output(t);
			free(data);
			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;
		case 6:
			printf("Enter a key: ");
			in=getInt(&key);
			if(in) return CERR_EOF;

			i=DelByKey(t, key);
			if(i==ERR_NO_FOUND) printf("Error. No such key in table.\n");
			else output(t);

			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;	
		case 7:
			printf("Enter a key: ");
			in=getInt(&key);
			if(in) return CERR_EOF;

			printf("Now enter a version: ");
			in=getInt(&rel);
			if(in) return CERR_EOF;

			i=DelByVersion(t, key, rel);
			if(i==ERR_NO_FOUND) printf("Error. No such key or version in table.\n");
			else output(t);

			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;
		case 8:
			scanf("%*c");
			printf("Enter a filename where table will be saved: ");
			char* fn=enter();
			Table* p=TableWrite(t, fn);
			if(!p) printf("Error. Wrong filename.\n");
			else printf("Modified table was saved at %s\n", fn);
			free(fn);
			
			printf("Input any value to continue or EOF to stop.\n");
			cont=scanf("%d", &i);
			scanf("%*[^\n]");
			if(cont==EOF) return CERR_EOF;
			break;
	}
}

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
		//output(t);
		//printf("Press any key to continue or ctrl+d to stop program.\n");
		//int in=scanf("%s");
		//if(in==EOF)return NULL; 
		free(FileName);
		FileName=NULL;
		return t;
		//printf("Data: %d", msize);
	}while(1);
}
