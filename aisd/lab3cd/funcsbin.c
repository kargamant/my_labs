#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "HashLib/HashBin.h"
#include "HashLib/Hash.h"
#include "funcsbin.h"

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

int console(int p, Table* t)
{
	int (*view[])(Table*)={Newv, Inputv, Outv, Searchkv, Searchvv, Addv, Delkv, Delvv, KeyTr, Savev, Iteratekv};
	view[p](t);
}

int KeyTr(Table* t)
{
	printf("Enter a key: ");
	int key=0;
	int in=getInt(&key);
	if(in) return CERR_EOF;
	
	if(t->msize==0) 
	{
		printf("Error. Table was not initialized.\n");
		return EndView();
	}
	printf("\nKey hash info based on current table:\n");
	printf("h1: %d | h2: %d\n", h1(key, t->msize), h2(key, t->msize));
	printf("\nKey trajectory:\n");
	printf("research number: ");
	for(int i=0; i<t->msize; i++) printf("%d ", i);
	printf("\nvalue of hash:   ");
	for(int i=0; i<t->msize; i++)
	{
		printf("%d ", h(key, i, t->msize));
	}
	printf("\n");
	return EndView();
}

int Newv(Table* t)
{
	Savev(t);
	if(t->fd) fclose(t->fd);
	if(t->msize!=0 || t->fd) erased(t);
	printf("Enter msize of your table: ");
	int msize=0;
	int in=0;
	do
	{
		in=getInt(&msize);
		if(msize<=0) printf("Error. Size of table must be positive. Try again.\n");
	}while(msize<=0);
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

	printf("Now enter a filename of creating file: ");
	char* fd=enter();
	if(fcheck(fd)==CERR_EOF) 
	{
		free(fd);
		fd=NULL;
		return CERR_EOF;
	}	
	t->fd=fopen(fd, "w+b");	
	int re=New(msize, t);
	if(re!=ERR_OK) printf("Error. Wrong filename or wrong data in file.\n");
	printf("Table was succesfully initialized with msize=%d in file %s\n", msize, fd);
	
	free(fd);
	return EndView();
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

int Inputv(Table* t)
{
	Savev(t);
	do
	{	
		printf("Enter FileName: ");
		char* fnd=enter();
		if(fcheck(fnd)==CERR_EOF) return CERR_EOF;		
		FILE* check=fopen(fnd, "rb");
		if(check) fclose(check);
		else 
		{	
			printf("Warning. File did not exist. But it can be created for new table.\n");
			printf("Input \"yes\" if you want to create a new table in file that have been entered or any other value to return to menue: ");
			char* choice=enter();
			if(!strcmp(choice, "yes")) goto creating;
			else 
			{
				free(choice);
				free(fnd);
				fnd=NULL;
				choice=NULL;
				return CERR_OK;
			}
			creating:
			if(t->fd) fclose(t->fd);
			if(t->msize!=0 || t->fd) erased(t);
			printf("Enter msize of your table: ");
			int msize=0;
			int in=0;
			do
			{
				in=getInt(&msize);
				if(msize<=0) printf("Error. Size of table must be positive. Try again.\n");
			}while(msize<=0);
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
			t->fd=fopen(fnd, "w+b");	
			int re=New(msize, t);
			if(re!=ERR_OK) printf("Error. Wrong filename or wrong data in file.\n");
			printf("Table was succesfully initialized with msize=%d in file %s\n", msize, fnd);
			
			free(fnd);
			free(choice);
			return EndView();	
		}
		if(t->msize!=0) erased(t);
		if(t->fd) fclose(t->fd);
		t->fd=fopen(fnd, "r+b");
		fseek(t->fd, 0, SEEK_SET);

		int res=input("\0", t);
		if(res!=ERR_OK)
		{
			printf("Error. Got wrong data while parsing. Try again.\n");
			free(fnd);
			fnd=NULL;
			continue;
		}	
		free(fnd);
		fnd=NULL;
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

	Table* result=SearchByKey(t, key);
	if(result)
	{
		output(result);	
		erased(result);	
		free(result);
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
		printf("Error. Table is full.\n");
	}
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

int Savev(Table* t)
{
	if(!t->fd) return CERR_EOF;	
	int p=TableWrite(t);
	if(p!=ERR_OK) printf("Error with fwrite or there is no such file.\n\n");
	else printf("Modified table data was successfully saved\n\n");
	//free(fnd);
	
	/*
	printf("Enter a filename where table will be saved: ");
	char* fn=enter();
	if(!fn) return CERR_EOF;
	if(*fn==0)
	{
		free(fn);
		fn=NULL;
		fn=enter();
	}
			
	int p=TableWrite(t, fn);
	if(p==ERR_FIL) printf("Error. Wrong filename.\n");
	else printf("Modified table was saved at %s\n", fn);
	free(fn);*/
	//return EndView();
	return CERR_EOF;
}

int Iteratekv(Table* t)
{
	int key=0;
	printf("Enter a key: ");
	int in=getInt(&key);
	if(in) return CERR_EOF;

	KeySpace* res=get_rel(t, key);
	if(res)
	{
		Table* rt=create();
		rt->fd=t->fd;
		rt->msize=1;
		rt->ks=res;
		output(rt);
		//erased(rt);
		free(rt);
	}
	else printf("Error. Iteration was unsuccsessful.\n");

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
