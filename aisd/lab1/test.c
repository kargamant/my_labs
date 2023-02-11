#include <stdlib.h>
#include "structs.h"
#include <stdio.h>
#include "funcs.h"

int main()
{
	//input of matrix
	matr matrix={0, NULL};
	if(input(&matrix)) 
	{
		erase(matrix);
		return 1;
	}
	
	//solution
	int* b=(int*)calloc(matrix.m, matrix.m*sizeof(int));
	form_vector(b, matrix);	
	
	//Output and erasing
	printf("\nResult vector b: ");
	for(int* gr=b; gr-b<matrix.m; ++gr) printf("%d ", *gr);
	output(matrix);
	erase(matrix);
	free(b);
	b=NULL;
	return 0;
}
