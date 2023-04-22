#include <stdio.h>
#include <stdlib.h>
#include "BinTree.h"
#include "funcs.h"
#include <time.h>
#include <string.h>
#include <stdarg.h>

void generate(Node* root, int n, long long int limit, long long int str_limit)
{
	srand(time(NULL));
	for(int i=0; i<n; i++)
	{
		int key=rand()%limit;
		int size=rand()%str_limit;
		while(size==0) size=rand()%str_limit;
		char* info=malloc(size*sizeof(char));
		info[size-1]=0;
		for(char* ptr=info; ptr-info<size-1; ++ptr) *ptr=65+rand()%27;
		int result=AddNode(root, key, info);
		while(result!=ERR_OK)
		{
			key=rand()%limit;
			result=AddNode(root, key, info);
		}
	}
}

void timing(Node* root, )
{
}
