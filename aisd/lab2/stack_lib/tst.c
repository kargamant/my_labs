#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Enter size of stack: ");
	int n;
	scanf("%d", &n);
	Stack* stk=create(n);
	push(stk, 'a');
	push(stk, 'b');
	push(stk, 'c');
	output(stk);
	printf("%c\n", pop(stk));
	printf("%c\n", pop(stk));
	printf("%c\n", pop(stk));
	output(stk);
	erase(stk);
	return 0;
}
