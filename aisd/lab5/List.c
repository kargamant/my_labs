#include <stdlib.h>
#include "List.h"

List* L_init()
{
	List* x=(List*)malloc(sizeof(List));
	x->head=NULL;
	x->tail=NULL;
	return x;
}

void push_back(List* L, int info)
{
	Node* x=(Node*)malloc(sizeof(Node));
	x->data=info;
	x->next=NULL;
	if(L->tail) L->tail->next=x;
	else L->head=x;
	L->tail=x;
}
