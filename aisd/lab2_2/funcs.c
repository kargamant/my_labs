#include <stdlib.h>
#include "stack_lib/stack_v.h"
#include "stack_lib/stack.h"
#include <stdio.h>
#include <string.h>

//string parsing
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
	if(strlen(ptr)==1 && isOper(*ptr)) return 0;
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
char* form_inf(char* pref, Stk* exp, Stk* oper)
{	
	//p - indicator shows if there was stack error somewhere
	//info about error types in stack_lib/stack.h
	int p=ERR_OK;
 
	//h - amount of allocated stack elements that should be freed before return
	int h=0;

	//filling stack exp with strings: pref[i] + '\0'
	for(int i=0; i<strlen(pref); i++) 
	{
		char* t=(char*)malloc(2*sizeof(char));
		*t=pref[i];
		t[1]='\0';
		p=push(exp, t);
		h++;
		if(p==ERR_FULL)
		{
			h--;
			free(t);
			t=NULL;
			break;
		}
	}

	//if stack is full freeing alloc'd memory and return
	if(p==ERR_FULL) 
	{	
		for(int i=0; i<h; i++) free(pop(exp));
		printf("ERR_FULL. Stack exp is full.\n");
		return NULL;
	}
	
	//c - current element of exp
	char* c=pop(exp);
	if(!c)
	{
		printf("ERR_EMPTY. Stack exp is empty.\n");
		return NULL;
	}

	//k - indicator of stack oper
	int k=0;

	//main cycle of the solution
	while(c)
	{
		//if it is operand simply put it into oper
		if(isOper(*c)) 
		{
			p=push(oper, c);
			k++;
			if(p==ERR_FULL) 
			{
				k--;
				free(c);
				c=NULL;
				//h--
				printf("ERR_FULL. Stack oper is full.\n");
				break;
			}
		}
		else
		{
			//forming new part of expression and pushing it to oper
			char* a=pop(oper);
			char* b=pop(oper);
			int len_a=strlen(a);
			int len_b=strlen(b);

			//case with brackets
			if(*c!='/' && !isEmpty(exp))
			{
				char* adding=(char*)calloc(len_a+len_b+4, len_a+len_b+4);
				adding[0]='(';
				for(int i=1; i<len_a+1; i++) adding[i]=a[i-1];
				adding[len_a+1]=*c;
				for(int i=len_a+2; i<len_a+len_b+2; i++) adding[i]=b[i-len_a-2];
				adding[len_a+len_b+2]=')';
				p=push(oper, adding);	
			}

			//without brackets
			else
			{
				char* adding=(char*)calloc(len_a+len_b+2, len_a+len_b+2);
				for(int i=0; i<len_a; i++) adding[i]=a[i];
				adding[len_a]=*c;
				for(int i=len_a+1; i<len_a+len_b+1; i++) adding[i]=b[i-len_a-1];
				p=push(oper, adding);	
			}	
			free(a);
			free(b);
			free(c);
			c=NULL;
			a=NULL;
			b=NULL;	
			if(p==ERR_FULL) break;
		}
		c=pop(exp);
		h--;
	}
	if(p==ERR_FULL)
	{
		//cleaning alloc'd strings
		for(int i=0; i<k; i++) free(pop(oper));
		for(int i=0; i<h; i++) free(pop(exp));
		return NULL;
	}
	char* result=pop(oper);	
	return result;
}
