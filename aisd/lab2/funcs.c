#include <stdlib.h>
#include "stack_lib/stack_v.h"
#include "stack_lib/stack.h"
#include <stdio.h>
#include <string.h>

char* enter()
{
	//printf("Enter string: ");
	char buf[81]={0};
	char* ptr=(char*)malloc(1);
	*ptr='\0';
	int n, len=0;
	do
	{
		n=scanf("%80[^\n]", buf);
		if(n<0) 
		{
			//printf("Got EOF. Input stopped.\n");
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

int isSign(char c)
{
	if(c>=42 && c<=47 && c!=44 && c!=46) return 1;
	return 0;
}

int isOper(char c)
{
	if((c>=65 && c<=90) || (c>=97 && c<=122)) return 1;
	return 0;
}

/*
int check_exp(char* ptr)
{
	for(int i=0; i<strlen(ptr)-2; i+=3)
	{
		//(*gr>=65 && *gr<=90)||(*gr>=97 && *gr<=122)||(*gr>=42 && *gr<=47 && *gr!=44 && *gr!=46)
		if((ptr[i]>=42 && ptr[i]<=47 && ptr[i]!=44 && ptr[i]!=46) && (ptr[i+1]>=65 && ptr[i+2]<=90) && (ptr[i+2]>=97 && ptr[i+2]<=122)) continue;
		else return 1;
	}
	return 0;
}*/
int mystrlen(char* s)
{
	char* ptr=s;
	while(ptr) ++ptr;
	return ptr-s;
}

char* form_inf(char* pref)
{
	Stk* exp=create(strlen(pref));
	Stk* oper=create(3);
	for(int i=0; i<strlen(pref); i++) 
	{
		char* t=calloc(2, 2*sizeof(char));
		*t=pref[i];
		push(exp, t);
	}
	char* c=pop(exp);
	while(c)
	{
		if(isOper(*c)) push(oper, c);
		else
		{
			char* a=pop(oper);
			char* b=pop(oper);
			int len_a=strlen(a);
			int len_b=strlen(b);
			if(*c!='*' && *c!='/' && !isEmpty(exp))
			{
				char* adding=(char*)calloc(len_a+len_b+4, len_a+len_b+4);
				adding[0]='(';
				for(int i=1; i<len_a+1; i++) adding[i]=a[i-1];
				adding[len_a+1]=*c;
				for(int i=len_a+2; i<len_a+len_b+2; i++) adding[i]=b[i-len_a-2];
				adding[len_a+len_b+2]=')';
				push(oper, adding);
			}
			else
			{
				char* adding=(char*)calloc(len_a+len_b+2, len_a+len_b+2);
				for(int i=0; i<len_a; i++) adding[i]=a[i];
				adding[len_a]=*c;
				for(int i=len_a+1; i<len_a+len_b+1; i++) adding[i]=b[i-len_a-1];
				push(oper, adding);
			}
			free(a);
			free(b);
			free(c);
			c=NULL;
			a=NULL;
			b=NULL;	
		}
		c=pop(exp);
	}
	char* result=pop(oper);
	erase(exp);
	erase(oper);
	return result;
}
