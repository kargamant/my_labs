#include "stack_v.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//allocating block of memory for stack
Stk* create(int n)
{
	if(n==0) return NULL;
	//calloc -> malloc
	Stk* stk=(Stack*)calloc(1, sizeof(Stack));
	stk->top=0;
	stk->n=n;
	stk->stk=(char**)calloc(n, n*sizeof(char*));
	return stk;
}

//bool func for full stack
int isFull(Stk* stk)
{
	return stk->top==stk->n;
}

//bool func for empty stack
int isEmpty(Stk* stk)
{
	return stk->top==0;
}

//pushing item to the top of the stack
int push(Stk* stk, char* a)
{
	if(isFull(stk)) return ERR_FULL;
	stk->stk[stk->top]=a;
	stk->top++;
	return ERR_OK;
}

//taking element from the top of the stack
char* pop(Stk* stk)
{
	if(stk->top==0) return NULL; 
	stk->top--;
	return stk->stk[stk->top];
}

//erasing stack and it's elements
void erase(Stk* stk)
{
	//for(int i=0; i<stk->top; i++) free(stk->stk[i]);
	free(stk->stk);
	stk->stk=NULL;
	free(stk);
}

//output
void output(Stk* stk)
{
	for(int i=0; i<stk->top; i++) printf("%s ", stk->stk[i]);	
	printf("\n");
}
