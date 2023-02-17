#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Enter size of stack: ");
	int n;
	scanf("%d", &n);
	Stk* stk=create(n);
	push(stk, 'a');
	push(stk, '+');
	if(push(stk, 'b')==ERR_FULL) printf("Stack is full\n");
	output(stk);
	char c=pop(stk);
	if(c!=ERR_EMPTY) printf("%c\n", c);
	else printf("Stack is empty\n");
	output(stk);
	c=pop(stk);
	if(c!=ERR_EMPTY) printf("%c\n", c);
	else printf("Stack is empty\n");
	output(stk);
	c=pop(stk);
	if(c!=ERR_EMPTY) printf("%c\n", c);
	else printf("Stack is empty\n");
	output(stk);
	push(stk, 'k');
	output(stk);
	erase(stk);
	return 0;
}
