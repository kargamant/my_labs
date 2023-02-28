#include <stdlib.h>
#include "stack_lib/stack_v.h"
#include "stack_lib/stack.h"
#include <stdio.h>
#include <string.h>

//string parsing
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

//bool func for sign
int isSign(char c)
{
	return (c>=42 && c<=47 && c!=44 && c!=46);
}

//bool func for operand
int isOper(char c)
{
	return ((c>=65 && c<=90) || (c>=97 && c<=122));
}

//checking if checked expression is not infix, postfix or smth else
int is_prefix(char* ptr)
{
	return isSign(ptr[0]) && isOper(ptr[strlen(ptr)-1]) && isOper(ptr[strlen(ptr)-2]);	
}

//checking basic correctness of expression
int check_exp(char* ptr)
{
	int si=0, op=0;
	for(char* gr=ptr; gr-ptr<strlen(ptr); ++gr)
	{
		op+=isOper(*gr);
		si+=isSign(*gr);
		if(!isSign(*gr)&& !isOper(*gr)) return 1;
	}
	if((op-si)!=1) return 1;
	else if(!is_prefix(ptr)) return 1;
	return 0;
}

//stack solution with 2 stacks
char* form_inf(char* pref)
{
	//stack of whole expression
	Stk* exp=create(strlen(pref));
	int signs=0;
	for(int i=0; i<strlen(pref); i++) 
	{
		char* t=calloc(2, 2*sizeof(char));
		*t=pref[i];
		push(exp, t);
		signs+=isOper(*t);
	}

	//stack to store operands
	Stk* oper=create(signs);
	char* c=pop(exp);
	while(c)
	{
		if(isOper(*c)) push(oper, c);
		else
		{
			//forming new part of expression and pushing it to oper
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
