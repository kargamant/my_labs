#include <stdlib.h>
#include "structs.h"
#include <stdio.h>
#include "funcs.h"

int main()
{
	//input of matrix
	int in;
	matr matrix={0, NULL};
	in=input(&matrix);
	if(in) 
	{
		erase(matrix);
		return 1;
	}
	
	//solution
	int* b=(int*)calloc(matrix.m, matrix.m*sizeof(int));
	form_vector(b, matrix);	
	
	//Output and erasing
	printf("\nResult vector b: ");
	for(int i=0; i<matrix.m; i++) printf("%d ", b[i]);
	const char message[13]="\nyour matrix:";
	output(message, matrix);
	erase(matrix);
	free(b);
	b=NULL;
	return 0;
}
