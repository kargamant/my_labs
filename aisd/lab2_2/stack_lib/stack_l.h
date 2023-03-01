#ifndef STACK_L
#define STACK_L
#include "list.h"

typedef struct Stack
{
	Item* top;
	List* stk;
}Stack;

#endif
