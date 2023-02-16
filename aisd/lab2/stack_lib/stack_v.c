#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack* create(int n)
{
	Stack* stk=(Stack*)calloc(1, sizeof(Stack));
	stk->top=0;
	stk->n=n;
	stk->stk=(char*)calloc(n, n*sizeof(char));
	return stk;
}

int isFull(Stack* stk)
{
	return stk->top==stk->n;
}

int isEmpty(Stack* stk)
{
	return stk->top==0;
}

int push(Stack* stk, char a)
{
	if(isFull(stk))
	{
		printf("stack is full\n");
		return 1;
	}
	stk->stk[stk->top]=a;
	stk->top++;
	return 0;
}

char pop(Stack* stk)
{
	if(isEmpty(stk)) 
	{
		printf("stack is empty\n");
		return '\0';
	}
	stk->top--;
	return stk->stk[stk->top];
}

void erase(Stack* stk)
{
	free(stk->stk);
	stk->stk=NULL;
	free(stk);
	stk=NULL;
}

void output(Stack* stk)
{
	for(char* gr=stk->stk; gr-stk->stk<stk->top; ++gr) printf("%c ", *gr);
	printf("\n");
}
