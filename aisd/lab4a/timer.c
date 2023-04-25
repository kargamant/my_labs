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

void timing(long long nodes, long long limit, long long str_limit, int itr)
{
	system("mkdir timing");
	printf("\n");
	const char* result_files[]=
	{
		"timing/AddNode.txt",
		"timing/Traversing.txt",
		"timing/DelNode.txt",
		"timing/Search.txt",
		"timing/Max.txt",
	};
	
	
	TimingTable* tt=(TimingTable*)malloc(sizeof(TimingTable));
	tt->msize=5;
	tt->csize=0;
	tt->info=(Item*)calloc(tt->msize, tt->msize*sizeof(Item));

	
	for(Item* ptr=tt->info; ptr-tt->info<tt->msize; ++ptr) 
	{
		ptr->fn=result_files[ptr-tt->info];
	}
	
	if(nodes>limit)
	{
		printf("Warning. Key limit is less than amount of nodes. That means that it isn't possible to generate such tree.\n");
		printf("Key limit was increased to value=nodes*2\n");
		limit=nodes*2;
	}
	long long fnodes=nodes;
	long double trave_t=0, max_t=0, search_t=0, add_t=0, del_t=0; 
	srand(time(NULL));
	Node* root=(Node*)calloc(1, sizeof(Node));
	while(nodes!=0)
	{
		generate(root, nodes, limit, str_limit);
		long double t1=0, t2=0;


		for(int i=0; i<itr; i++)
		{
			//Testing of AddNode
			int key=rand()%limit;
			int size=rand()%str_limit;
			char* info=(char*)calloc(size, size*sizeof(char));
			for(char* ptr=info; ptr-info<size-1; ++ptr) *ptr=65+32*(rand()%2)+rand()%27;
			t1=clock();
			int result=AddNode(root, key, info);
			t2=clock();
			if(result!=ERR_OK) free(info);
			add_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);

			//Testing of Traversing
			t1=clock();
			result=Traversing_no_print(root, -1);
			t2=clock();
			trave_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);

			//Testing of Delition	
			key=rand()%limit;
			t1=clock();
			result=DelNode(root, key);
			t2=clock();
			del_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);

			//Testing of search
			key=rand()%limit;
			t1=clock();
			Node* node=Search(root, key);
			t2=clock();
			search_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);

			//Testing max
			t1=clock();
			Max(root);
			t2=clock();
			max_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);
		}
		add_t/=itr;
		trave_t/=itr;
		del_t/=itr;
		search_t/=itr;
		max_t/=itr;
		WriteTiming(tt, 0, result_files[0], nodes, add_t);	
		WriteTiming(tt, 1, result_files[1], nodes, trave_t);	
		WriteTiming(tt, 2, result_files[2], nodes, del_t);	
		WriteTiming(tt, 3, result_files[3], nodes, search_t);	
		WriteTiming(tt, 4, result_files[4], nodes, max_t);	

		for(Item* ptr=tt->info; ptr-tt->info<tt->msize; ++ptr) 
		{
			FILE* fd=fopen(ptr->fn, "a");
			if(nodes==fnodes)
			{
				fclose(fd);
				fd=fopen(ptr->fn, "w+");
			}
			fprintf(fd, "%lld %Lf\n", ptr->nodes, ptr->timing);
			fclose(fd);
		}
		nodes/=2;
		Node* start=Max(root);
		while(start!=NULL)
		{
			if(start->info) 
			{
				free(start->info);
				start->info=NULL;
			}
			Node* prev=start->prev;
			if(start!=root) free(start);
			start=prev;
		}
		root->info=NULL;
	}
	free(tt->info);
	free(tt);
	free(root);
}

Item* Searchtt(TimingTable* vector, int ms, char* fn)
{
	for(Item* ptr=vector->info; ptr-vector->info<ms; ++ptr)
	{
		if(!strcmp(ptr->fn, fn)) return ptr;
	}
	return NULL;
}

void WriteTiming(TimingTable* tt, int curf, const char* fn, int nodes, long double timing)
{
	tt->info[curf].fn=fn;
	tt->info[curf].nodes=nodes;
	tt->info[curf].timing=timing;
}


int Traversing_no_print(Node* root, int key)
{
	Node* ptr=NULL;
	if(root->info==NULL) return ERR_EMPTY;
	Node* mr=Max(root);
	if(key==FULL_TREE || key>mr->key) ptr=mr;
	else ptr=Search(root, key);
	if(!ptr) return ERR_NF;

	while(ptr!=NULL)
	{
		//printf("key: %d | info: %s\n", ptr->key, ptr->info);
		ptr=ptr->prev;
	}
	return ERR_OK;
}
