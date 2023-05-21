#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue
{
	int b;
	int e;
	int capacity;
	int size;
	int* queue;
}Queue;

/*typedef enum ERR
{
	ERR_OK,
	ERR_FULL,
	ERR_EMPTY
}ERR;*/

//methods

Queue* QueueInit(int capacity);
int push(Queue* Q, int n);
int pop(Queue* Q, int* value);
#endif
