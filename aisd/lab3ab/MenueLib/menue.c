#include <stdlib.h>
#include "../TableLib/Table.h"
#include "../funcs.h"
//interactive menue
int menue(const char* options[], int n)
{	
	for(int i=0; i<15; i++) printf("-");
	printf("\n");
	for(int i=0; i<n; i++)
	{
		printf("%d. %s", i, options[i]);
	}
	printf("\nEnter EOF to quit.\n");
	for(int i=0; i<15; i++) printf("-");
	printf("\n");

	printf("Choose option: \n");
	int pos=n+1;
	do
	{
		int in=getInt(&pos);
		if(in) return n+1;
		else if(pos<0 || pos>n)
		{
			printf("Incorrect number of option. Try again.\n");
			continue;
		}
		else break;
	}
	while(pos<0 || pos>n);
	return pos;
}
