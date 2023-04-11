#include <math.h>

int isPrime(int m)
{
	if(m==1) return 0;
	if(m==2) return 1;
	for(int i=2; i<sqrt(m)+1; i++)
	{
		if(m%i==0) return 0;
	}
	return 1;
}

int h1(int k, int m)
{
	return k%m;
}

//assuming m as prime number
int h2(int k, int m)
{
	return (1+k%(m-1))%m;
}

int h(int k, int i, int m)
{
	return (h1(k, m) + i*h2(k, m))%m;
}
