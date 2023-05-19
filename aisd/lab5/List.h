#ifndef LIST_H
#define LIST_H

typedef struct List
{
	int data;
	struct List* next;
}List;

void pop(List* L, int* u);
void push(List* L, int info);
List* L_init(int data);
#endif
