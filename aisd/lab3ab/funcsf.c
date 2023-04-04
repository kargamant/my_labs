#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TableLib/TableBin.h"
#include "TableLib/Table.h"
#include "funcs.h"

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
	int (*view[])(Table*)={Newv, Inputv, Outv, Searchkv, Searchvv, Addv, Delkv, Delvv, Savev, Rawv};
	view[p](t);
}

int Newv(Table* t)
{
	Savev(t);
	if(t->msize!=0 || t->fi || t->fd) erased(t);
	printf("Enter msize of your table: ");
	int msize=0;
	int in=0;
	do
	{
		in=getInt(&msize);
		if(msize<=0) printf("Error. Size of table must be positive. Try again.\n");
	}while(msize<=0);
	if(in) return CERR_EOF;
	printf("Now enter a filename of creating file: ");
	char* fd=enter();
	if(fcheck(fd)==CERR_EOF) 
	{
		free(fd);
		fd=NULL;
		return CERR_EOF;
	}
	printf("And enter a filename where info will be stored: ");
	char* fi=enter();
	if(fcheck(fi)==CERR_EOF)
	{
		free(fi);
		fi=NULL;
		return CERR_EOF;
	}
	t->fd=fd;
	t->fi=fopen(fi, "w+b");
	/*
	char* dir=(char*)malloc(6*sizeof(char));
	char f[6]="bins/\0";
	for(int i=0; i<6; i++) dir[i]=f[i];
	char* fd_dir=(char*)realloc(dir, strlen(dir)+strlen(fd)+1);
	strcat(fd_dir, fd);
	//printf("fd_dir: %s\n", fd_dir);
	t->fd=fd_dir;

	char* ndir=(char*)malloc(6*sizeof(char));
	for(int i=0; i<6; i++) ndir[i]=f[i];
	char* fi_dir=(char*)realloc(ndir, strlen(ndir)+strlen(fi)+1);
	strcat(fi_dir, fi);
	t->fi=fopen(fi_dir, "w+b");*/

	int re=New(msize, t);
	if(re!=ERR_OK) printf("Error. Wrong filename or wrong data in file.\n");
	printf("Table was succesfully initialized with msize=%d and files %s %s\n", msize, fd, fi);

	free(fi);
	//free(fi_dir);
	//free(fd);
	//fd=NULL;
	//fi_dir=NULL;
	//free(ndir);
	//ndir=NULL;
	//free(dir);
	//dir=NULL;
	fi=NULL;
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
		if(t->msize!=0) erased(t);
		printf("Enter data FileName: ");
		char* fnd=enter();
		if(fcheck(fnd)==CERR_EOF) return CERR_EOF;	
		printf("Now enter info FileName: ");
		char* fni=enter();
		if(fcheck(fni)==CERR_EOF) return CERR_EOF;
		//Using r+b, because if open in w+b file will be truncated.
		//File will remain, but all content will be removed.
		t->fd=fnd;
		FILE* check=fopen(t->fd, "rb");
		t->fi=fopen(fni, "rb");
		if(!t->fi || !check) 
		{
			if(check) fclose(check);
			if(t->fi) 
			{
				fclose(t->fi);
				t->fi=NULL;
			}
			printf("Warning. One of the files did not exist. But it can be created for new table.\n");
			printf("Input \"yes\" if you want to create a new table in files that have been entered or any other value to return to menue: ");
			char* choice=enter();
			if(!strcmp(choice, "yes")) goto creating;
			else 
			{
				free(choice);
				free(fni);
				free(fnd);
				fnd=NULL;
				fni=NULL;
				choice=NULL;
				t->fd=NULL;
				t->fi=NULL;
				return CERR_OK;
			}
			creating:
			if(!t->fi) t->fi=fopen(fni, "w+b");
			check=fopen(fnd, "w+b");
			fclose(check);
			printf("Enter msize of your new table: ");
			int msize=0;
			int in=getInt(&msize);
			if(in) return CERR_EOF;
			int re=New(msize, t);
			if(re!=ERR_OK) printf("Error. Wrong filename or wrong data in file.\n");
			printf("Table was succesfully initialized with msize=%d and files %s %s\n", msize, fnd, fni);
			free(fni);
			free(choice);
			choice=NULL;
			fni=NULL;
			return EndView();
		}
		fclose(check);
		fclose(t->fi);
		t->fi=fopen(fni, "r+b");
		rewind(t->fi);

		int res=input(fnd, t);
		if(res==ERR_FIL)
		{
			printf("File does not exist or wrong FileName. Try again.\n");
			free(fnd);
			fnd=NULL;
			free(fni);
			fni=NULL;
			continue;
		}	
		if(res==ERR_WRD)
		{
			printf("Error. Got wrong data while parsing. Try again.\n");
			free(fnd);
			fnd=NULL;
			free(fni);
			fni=NULL;
			continue;
		}
		//free(fnd);
		free(fni);
		//fnd=NULL;
		fni=NULL;
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
	if(result) outputksf(result, t->fi);
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
		outputndf(res, t->fi);
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

	in=addf(t, key, data, "\0");
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
	/*printf("Enter data FileName: ");
	char* fnd=enter();
	if(fcheck(fnd)==CERR_EOF) return CERR_EOF;*/	
	if(!t->fd) return CERR_EOF;	
	int p=TableWrite(t, "\0");
	if(p==ERR_FWRITE || p==ERR_FIL) printf("Error with fwrite or there is no such file.\n\n");
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

int Rawv(Table* t)
{
	if(!t->fi || !t->fd) 
	{
		printf("Error. No files created for this table.\n");
		return EndView();
	}
	printf("Raw data from data file: ");
	//FILE* fd=fopen(t->fd, "r+b");
	FILE* fd=fopen(t->fd, "r+b");
	int msize, key, m;
	fread(&msize, sizeof(int), 1, fd);
	printf("%d|", msize);
	for(int i=0; i<t->csize; i++)
	{
		fread(&key, sizeof(int), 1, fd);
		fread(&m, sizeof(int), 1, fd);
		printf("%d|%d|", key, m);
		for(int j=0; j<m; j++)
		{
			int rel, offset, len;
			fread(&rel, sizeof(int), 1, fd);
			fread(&offset, sizeof(int), 1, fd);
			fread(&len, sizeof(int), 1, fd);
			printf("%d|%d|%d|", rel, offset, len);
		}
	}
	fclose(fd);
	printf("\nRaw info from info file: ");
	KeySpace* ptr=t->ks;
	while(ptr-t->ks<t->csize)
	{
		Node* gr=ptr->Node;
		while(gr)
		{
			//FILE* fi=fopen(t->fi, "r+b");
			FILE* fi=t->fi;
			char* info=(char*)malloc(gr->len*sizeof(char));
			fseek(fi, gr->offset, SEEK_SET);
			fread(info, sizeof(char), gr->len, fi);
			printf("%s|", info);
			free(info);
			info=NULL;
			gr=gr->next;
			//fclose(fi);
		}
		++ptr;
	}
	printf("\n");
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
