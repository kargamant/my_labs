#ifndef LIST_H
#define LIST_H

typedef struct List
{
	int data;
	struct List* next;
}List;

void popl(List* L, int* u);
void push_front(List* L, int info);
List* L_init(int data);
#endif
