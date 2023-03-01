#include "list.h"
#include "stack_l.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

//allocating block of memory for stack structure
Stk* create(int n)
{
	Stk* stk=(Stk*)calloc(1, sizeof(Stk));
	stk->stk=create_list();
	stk->top=stk->stk->end;
	return stk;
}

//bool func for empty stack
int isEmpty(Stk* stk)
{
	return stk->top==NULL;
}

//pushing element to the top of the stack
int push(Stk* stk, char* a)
{
	push_l(stk->stk, a);
	stk->top=stk->stk->head;
	return ERR_OK;
}

//taking element from the top of the stack
char* pop(Stk* stk)
{
	if(stk->top==NULL) return NULL;
	char* value = stk->top->c;
	Item* new_top=stk->top->next;
	free(stk->top);
	stk->top=new_top;
	stk->stk->head=stk->top;
	return value;
}

//erasing stack and it's items
void erase(Stk* stk)
{
	del(stk->stk);
	free(stk);
	stk=NULL;
}

//output
void output(Stk* stk)
{
	show(stk->stk);
}
