#ifndef STACK_L
#define STACK_L
#include "list.h"

typedef struct Stack
{
	Item* top;
	int n;//size of list
	int now;//elements now in list
	List* stk;
}Stack;

#endif
