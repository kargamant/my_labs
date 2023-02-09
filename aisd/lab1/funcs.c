#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

int getInt(int* n)
{
	int input;
	do
	{
		input=scanf("%d", n);
		if(input==EOF) return 1;
		else if(input==0)
		{
			printf("Error, wrong data. Try again\n");
			scanf("%*[^\n]"); 
		}
		else return 0;
	}while(input==0);
}

int number_check(int* m)
{
	int input;
	do
	{
		input=getInt(m);
		if(input)
		{
			printf("Error, got EOF instead of number\n");
			return 1;
		}
		else if(*m<0) printf("Error, got negative value, expected positive\n");
	}while(*m<0);
	return 0;
}

int input(matr* matrix)
{
	int m, n, input;
	printf("Enter number of lines:");
	input=number_check(&m);
	if(input) return 1;
	matrix->m=m;
	matrix->matr=(Line*)malloc(m*sizeof(Line));
	for(int i=0; i<m; i++)
	{
		printf("\nEnter number of elements in line %d:", i+1);
		input=number_check(&n);
		if(input) return 1;
		matrix->matr[i].n=n;
		matrix->matr[i].line=(int*)malloc(n*sizeof(int));

		if(n!=0) printf("Enter elements of line: ");
		for(int j=0; j<n; j++) getInt(matrix->matr[i].line+j);
	}
	return 0;
}

void output(const char* message, matr matrix)
{
	printf("%s\n", message);
	for(int i=0; i<matrix.m; i++)
	{
		for(int j=0; j<matrix.matr[i].n; j++)
		{
			printf("%d ", matrix.matr[i].line[j]);
		}
		printf("\n");
	}
}

void erase(matr matrix)
{
	for(int i=0; i<matrix.m; i++)
	{
		free(matrix.matr[i].line);
	}
	free(matrix.matr);
}
