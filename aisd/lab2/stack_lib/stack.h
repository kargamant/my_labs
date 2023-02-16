#ifndef STACK 
#define STACK

typedef struct Stack
{
	int top;
	int n;
	void* stk;
}Stack;
Stack* create(int n);
int push(Stack* stk, int a);
char pop(Stack* stk);
int getTop(Stack* stk);
int isFull(Stack* stk);
int isEmpty(Stack* stk);

#endif
