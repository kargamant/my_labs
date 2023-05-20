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

//base hash function
int h1(char* k, int m)
{
	int s=0;
	char* c=k;
	while(*c!=0)
	{
		s=s+*c;
		++c;
	}
	return s%m;
}

//step hash function
//assuming m as prime number. Otherwise it'll get into cycle.
int h2(char* k, int m)
{
	int s=0;
	char* c=k;
	while(*c!=0)
	{
		s=s+*c;
		++c;
	}
	return (1+s%(m-1))%m;
}

//iterator hash function
int h(char* k, int i, int m)
{
	return (h1(k, m) + i*h2(k, m))%m;
}
