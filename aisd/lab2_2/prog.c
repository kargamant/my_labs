#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack_lib/stack.h"
#include "funcs.h"

int main()
{
	int e=0, o=0;
	printf("Enter sizes of stacks exp and oper: ");
	scanf("%d %d", &e, &o);
	scanf("%*c");		

	do
	{
		printf("Enter prefix expression: ");
		char* ptr=enter();	
		if(!ptr) break;	
		if(check_exp(ptr))
		{
			printf("Wrong expresion. Try again.\n");
			free(ptr);
			ptr=NULL;
			continue;
		}

		char* result;

		//case: simple exp of one single operand
		if(strlen(ptr)==1) 
		{
			result=ptr;
			goto res;
		}

		Stk* exp=create(e);
		Stk* oper=create(o);
		result=form_inf(ptr, exp, oper);
		if(!result)
		{
			erase(exp);
			erase(oper);
			free(ptr);
			ptr=NULL;
			continue;
		}
		erase(exp);
		erase(oper);	
		res:
		printf("infix expression: %s\n", result);
		if(strlen(ptr)!=1) free(result);
		result=NULL;
		free(ptr);
		ptr=NULL;
	}while(1);
	printf("Got EOF. Programm stopped.\n");
	
	return 0;
}
