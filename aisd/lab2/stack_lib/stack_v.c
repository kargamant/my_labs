#include "stack.h"

int push(Stack* stk, int a)
{
	if(stk->top==stk->n - 1) return 1;
	stk->stk[stk->top]=a;
	stk->top++;
	return 0;
}

char pop(Stack* stk)
{
	if(stk->top==0) return 0;
	stk->top--;
	return stk->stk[top];
}
