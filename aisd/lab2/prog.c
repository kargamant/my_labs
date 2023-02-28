#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

int main()
{
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
		char* result=form_inf(ptr);
		printf("infix expression: %s\n", result);
		free(result);
		free(ptr);
		result=NULL;
		ptr=NULL;
	}while(1);
	printf("Got EOF. Programm stopped.\n");
	return 0;
}
