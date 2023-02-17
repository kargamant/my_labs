#include "stack_v.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stk* create(int n)
{
	if(n==0) return NULL;
	Stk* stk=(Stack*)calloc(1, sizeof(Stack));
	stk->top=0;
	stk->n=n;
	stk->stk=(char*)calloc(n, n*sizeof(char));
	return stk;
}

int push(Stk* stk, char a)
{
	if(stk->top==stk->n) return ERR_FULL;
	stk->stk[stk->top]=a;
	stk->top++;
	return ERR_OK;
}

char pop(Stk* stk)
{
	if(stk->top==0) return ERR_EMPTY; 
	stk->top--;
	return stk->stk[stk->top];
}

void erase(Stk* stk)
{
	free(stk->stk);
	stk->stk=NULL;
	free(stk);
	stk=NULL;
}

void output(Stk* stk)
{
	for(char* gr=stk->stk; gr-stk->stk<stk->top; ++gr) printf("%c ", *gr);
	printf("\n");
}
