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
		/*
		//Probably can count signs and letters. And compare their amount. 
		if(check_exp(ptr))
		{
			printf("Incorrect form of prefix expression.\nTry again.\n");
			continue;
		}*/
		//solution
		//printf("Your infix expression: \"%s\"\n");
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
