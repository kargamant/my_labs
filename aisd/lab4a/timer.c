#include <stdio.h>
#include <stdlib.h>
#include "BinTree.h"
#include "funcs.h"
#include <time.h>
#include "timer.h" 
#include <string.h>
#include <stdarg.h>

void generate(Node* root, int n, long long limit, long long str_limit)
{
	srand(time(NULL));
	for(int i=0; i<n; i++)
	{
		int key=rand()%limit;
		int size=rand()%str_limit;
		while(size==0) size=rand()%str_limit;
		char* info=malloc(size*sizeof(char));
		info[size-1]=0;
		for(char* ptr=info; ptr-info<size-1; ++ptr) *ptr=65+32*(rand()%2)+rand()%27;
		int result=AddNode(root, key, info);
		while(result!=ERR_OK)
		{
			key=rand()%limit;
			result=AddNode(root, key, info);
		}
	}
}

void timing(long long nodes, long long limit, long long str_limit, const char* res_dir)
{
	const char* result_files[]=
	{
		"AddNode.txt\0",
		"Traversing.txt\0",
		"DelNode.txt\0",
		"Search.txt\0",
		"Max.txt\0",
	};
	
	TimingTable* tt=(TimingTable*)malloc(sizeof(TimingTable));
	tt->msize=5;
	tt->csize=0;
	tt->info=(Item*)malloc(tt->msize*sizeof(Item));
	
	for(Item* ptr=tt->info; ptr-tt->info<tt->msize; ++ptr) ptr->fn=result_files[ptr-tt->info];


	double trave_t, max_t, search_t, add_t, del_t; 
	srand(time(NULL));
	int curf=0;
	Node* root=(Node*)malloc(sizeof(Node));
	while(nodes!=0)
	{
		generate(root, nodes, limit, str_limit);
		double t1, t2;


		//Testing of AddNode

		int key=rand()%limit;
		int size=rand()%str_limit;
		char* info=malloc(size*sizeof(char));
		info[size-1]=0;
		for(char* ptr=info; ptr-info<size-1; ++ptr) *ptr=65+32*(rand()%2)+rand()%27;

		t1=clock();
		int result=AddNode(root, key, info);
		t2=clock();

		while(result!=ERR_OK)
		{
			key=rand()%limit;
			t1=clock();
			result=AddNode(root, key, info);
			t2=clock();
		}
		WriteTiming(tt, curf, result_files[curf], nodes, (t2-t1)/CLOCKS_PER_SEC);	
		curf++;

		//Testing of Traversing
		
		t1=clock();
		result=Traversing(root, -1);
		t2=clock();
		if(result==ERR_EMPTY) continue;
		WriteTiming(tt, curf, result_files[curf], nodes, (t2-t1)/CLOCKS_PER_SEC);	
		curf++;

		//Testing of Delition
		
		key=rand()%limit;
		t1=clock();
		result=DelNode(root, key);
		t2=clock();
		if(result==ERR_EMPTY) continue;
		else
		{
			while(result!=ERR_OK)
			{
				key=rand()%limit;
				t1=clock();
				result=DelNode(root, key);
				t2=clock();
			}
		}
		WriteTiming(tt, curf, result_files[curf], nodes, (t2-t1)/CLOCKS_PER_SEC);	
		curf++;

		//Testing of search
		//Perhaps i'll add average timing for diffrent key values

		key=rand()%limit;
		t1=clock();
		Node* node=Search(root, key);
		t2=clock();
		WriteTiming(tt, curf, result_files[curf], nodes, (t2-t1)/CLOCKS_PER_SEC);	
		curf++;

		//Testing max 
		
		t1=clock();
		Max(root);
		t2=clock();
		WriteTiming(tt, curf, result_files[curf], nodes, (t2-t1)/CLOCKS_PER_SEC);	
		curf++;

		for(Item* ptr=tt->info; ptr-tt->info<tt->msize; ++ptr) 
		{
			FILE* fd=fopen(ptr->fn, "w");
			fprintf(fd, "%lld %lf", ptr->nodes, ptr->timing);
			fclose(fd);
		}
		nodes/=2;
		curf=0;
		Node* start=Max(root);
		while(start!=NULL)
		{
			if(start->info) free(start->info);
			Node* prev=start->prev;
			if(start!=root) free(start);
			start=prev;
		}
		root->info=NULL;
	}
	free(tt->info);
	free(tt);
	erased(root);
}

Item* Searchtt(TimingTable* vector, int ms, char* fn)
{
	for(Item* ptr=vector->info; ptr-vector->info<ms; ++ptr)
	{
		if(!strcmp(ptr->fn, fn)) return ptr;
	}
	return NULL;
}

void WriteTiming(TimingTable* tt, int curf, const char* fn, int nodes, double timing)
{
	tt->info[curf].fn=fn;
	tt->info[curf].nodes=nodes;
	tt->info[curf].timing=timing;
}
