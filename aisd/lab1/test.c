#include <stdlib.h>
#include "structs.h"
#include <stdio.h>
#include "funcs.h"

int main()
{
	int in;
	matr matrix;
	in=input(&matrix);
	if(in) return 1;
	//matrix.m=m;
	//matrix.matr=(Line*)malloc(m*sizeof(Line));

	const char message[13]="\nyour matrix:";
	output(message, matrix);
	erase(matrix);
	return 0;
}
