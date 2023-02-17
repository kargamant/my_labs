#ifndef STACK 
#define STACK

typedef struct Stack Stk;
Stk* create(int n);
int push(Stk* stk, char a);
char pop(Stk* stk);
void erase(Stk* stk);
void output(Stk* stk);
typedef enum ERR
{
	ERR_OK,
	ERR_EOF,
	ERR_FULL,
	ERR_EMPTY
}ERR;


#endif
