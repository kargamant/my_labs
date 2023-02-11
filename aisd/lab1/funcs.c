#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

//Correct input of integer
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

//sum of digits
int digit_sum(int num)
{
	int k=0;
	while(num!=0)
	{
		k+=num%10;
		num=num/10;
	}
	return k>0? k:-k;
}

//function for summing forming elements
int sum_line(Line* ptr, int k)
{
	int total=0;
	for(int* gr=ptr->line; gr-ptr->line<ptr->n; ++gr)
	{
		int s=digit_sum(*gr);
		if(s==k) total+=*gr;
	}
	return total;
}

//Main solution function. Forming vector.
void form_vector(int* b, matr matrix)
{
	for(Line* ptr=matrix.matr; ptr-matrix.matr<matrix.m; ++ptr)
	{
		if(ptr->n==0) continue;
		int k=digit_sum(*(ptr->line));	
		b[ptr-matrix.matr]=sum_line(ptr, k);
	}
}

//Checking if we got positive number or is it negative or end of file
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

//Input of matrix
int input(matr* matrix)
{
	//input amount of lines and allocating memory for lines
	int m, n;
	printf("Enter number of lines:");
	if(number_check(&m)) return 1;
	matrix->m=m;
	if(m==0) return 0;
	matrix->matr=(Line*)calloc(m, sizeof(Line));
	
	//input size of every line, allocating and input elements of line
	for(Line* ptr=matrix->matr; ptr-matrix->matr<m; ++ptr)
	{
		printf("\nEnter number of elements in line %d:", ptr-matrix->matr+1);
		if(number_check(&n)) return 1;
		ptr->n=n;
		if(n==0) continue;
		ptr->line=(int*)calloc(n, sizeof(int));

		printf("Enter elements of line: ");
		for(int* gr=ptr->line; gr-ptr->line<n; ++gr) getInt(gr);
	}
	return 0;
}

//Output of matrix
void output(matr matrix)
{
	printf("\nyour matrix:\n");
	for(Line* ptr=matrix.matr; ptr-matrix.matr<matrix.m; ++ptr)
	{
		for(int* gr=ptr->line; gr-ptr->line<ptr->n; ++gr) printf("%d ", *gr);
		printf("\n");
	}
}

//Freeing space used by matrix
void erase(matr matrix)
{
	for(Line* ptr=matrix.matr; ptr-matrix.matr<matrix.m; ++ptr) free(ptr->line);
	free(matrix.matr);
}
