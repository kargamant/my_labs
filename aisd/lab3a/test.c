#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

int main()
{

	printf("Enter a name of file: ");
	char* fn=enter();
	
	free(fn);
	fn=NULL;
	return 0;
}
