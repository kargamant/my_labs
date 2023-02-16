#ifndef STACK 
#define STACK

typedef struct Stack
{
	int top;
	int n;
	char* stk;
}Stack;
Stack* create(int n);
int push(Stack* stk, char a);
char pop(Stack* stk);
int isFull(Stack* stk);
int isEmpty(Stack* stk);
void erase(Stack* stk);
void output(Stack* stk);

#endif
