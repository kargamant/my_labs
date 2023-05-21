#include <stdlib.h>
#include "Queue.h"
#include "Graph.h"

Queue* QueueInit(int capacity)
{
	Queue* Q=(Queue*)malloc(sizeof(Queue));
	Q->b=0;
	Q->e=0;
	Q->size=0;
	Q->capacity=capacity;
	Q->queue=(int*)malloc(capacity*sizeof(int));
	return Q;
}

int push(Queue* Q, int n)
{
	if(Q->size==Q->capacity) return ERR_FULL;
	else
	{
		Q->queue[Q->e]=n;
		Q->e=(Q->e+1)%Q->capacity;
		Q->size+=1;
	}
	return ERR_OK;
}

int pop(Queue* Q, int* value)
{
	if(Q->size==0) return ERR_EMPTY;
	else
	{
		*value=Q->queue[Q->b];
		Q->b=(Q->b+1)%Q->capacity;
		Q->size-=1;
	}
	return ERR_OK;
}
