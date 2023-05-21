#ifndef LIST_H
#define LIST_H

typedef struct Node
{
	int data;
	struct Node* next;
}Node;

typedef struct List
{
	Node* head;
	Node* tail;
}List;

void push_back(List* L, int info);
List* L_init();
#endif
