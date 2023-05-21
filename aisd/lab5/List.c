#include <stdlib.h>
#include "List.h"

List* L_init(int data)
{
	List* x=(List*)malloc(sizeof(List));
	x->data=data;
	x->next=NULL;
	return x;
}

void push_front(List* L, int info)
{
	List* x=L_init(info);
	x->next=L;
	L=x;
}

void popl(List* L, int* u)
{
	*u=L->data;
	L=L->next;
}
