#include "list.h"
#include "stack_l.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Stk* create(int n)
{
	Stk* stk=(Stk*)calloc(1, sizeof(Stk));
	stk->stk=create_list();
	stk->top=stk->stk->end;
	return stk;
}

int push(Stk* stk, char a)
{
	push_l(stk->stk, a);
	stk->top=stk->stk->head;
	return ERR_OK;
}

char pop(Stk* stk)
{
	if(stk->top==NULL) return ERR_EMPTY;
	char value = stk->top->c;
	Item* new_top=stk->top->next;
	free(stk->top);
	stk->top=new_top;
	stk->stk->head=stk->top;
	return value;
}

void erase(Stk* stk)
{
	del(stk->stk);
	free(stk);
	stk=NULL;
}

void output(Stk* stk)
{
	show(stk->stk);
}
