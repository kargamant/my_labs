#include "list.h"
#include "stack_l.h"
#include "stack.h"

Stk* create(int n)
{
	if(n==0) return NULL;
	Stk* stk=(Stack*)calloc(1, sizeof(Stack));
	stk->n=n;
	stk->stk=create_list();
	stk->top=stk->stk->end;
	return stk;
}

int push(Stk* stk, char a)
{
	if(stk->	
}
