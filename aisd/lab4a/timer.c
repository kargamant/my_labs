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

void timing(long long nodes, long long limit, long long str_limit, const char* res_dir, int itr)
{
	const char* result_files[]=
	{
		"AddNode.txt",
		"Traversing.txt",
		"DelNode.txt",
		"Search.txt",
		"Max.txt",
	};
	
	TimingTable* tt=(TimingTable*)malloc(sizeof(TimingTable));
	tt->msize=5;
	tt->csize=0;
	tt->info=(Item*)malloc(tt->msize*sizeof(Item));
	
	for(Item* ptr=tt->info; ptr-tt->info<tt->msize; ++ptr) 
	{
		ptr->fn=result_files[ptr-tt->info];
	}

	//for(int i=0; i<5; i++) printf("%s\n", tt->info[i]);
	long long fnodes=nodes;
	double trave_t, max_t, search_t, add_t, del_t; 
	srand(time(NULL));
	int curf=0;
	Node* root=(Node*)malloc(sizeof(Node));
	while(nodes!=0)
	{
		generate(root, nodes, limit, str_limit);
		double t1, t2;


		for(int i=0; i<itr; i++)
		{
			//Testing of AddNode
			int key=rand()%limit;
			int size=rand()%str_limit;
			char* info=malloc(size*sizeof(char));
			info[size-1]=0;
			for(char* ptr=info; ptr-info<size-1; ++ptr) *ptr=65+32*(rand()%2)+rand()%27;

			t1=clock();
			int result=AddNode(root, key, info);
			t2=clock();

			/*while(result!=ERR_OK)
			{
				//printf("db34\n");
				key=rand()%limit;
				t1=clock();
				result=AddNode(root, key, info);
				t2=clock();
			}*/
			add_t+=(t2-t1)/CLOCKS_PER_SEC;
			//WriteTiming(tt, curf, result_files[curf], nodes, (t2-t1)/CLOCKS_PER_SEC);	
			//curf++;

			//Testing of Traversing
			
			t1=clock();
			result=Traversing_no_print(root, -1);
			t2=clock();
			/*if(result==ERR_EMPTY) 
			{
				curf=0;
				continue;
			}*/
			trave_t+=(t2-t1)/CLOCKS_PER_SEC;

			//Testing of Delition
			
			key=rand()%limit;
			t1=clock();
			result=DelNode(root, key);
			t2=clock();
			/*if(result==ERR_EMPTY) 
			{
				curf=0;
				continue;
			}*/
			//printf("db1\n");
			/*if(result==ERR_NF)
			{
				while(result!=ERR_OK)
				{
					//printf("db3\n");
					key=rand()%limit;
					t1=clock();
					result=DelNode(root, key);
					t2=clock();
				}
			}*/
			del_t+=(t2-t1)/CLOCKS_PER_SEC;

			//Testing of search

			key=rand()%limit;
			t1=clock();
			Node* node=Search(root, key);
			t2=clock();
			search_t+=(t2-t1)/CLOCKS_PER_SEC;

			//Testing max 
			
			t1=clock();
			Max(root);
			t2=clock();
			max_t+=(t2-t1)/CLOCKS_PER_SEC;
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
			FILE* fd=NULL;
			if(nodes==fnodes)
			{
				fd=fopen(ptr->fn, "w+");
				fclose(fd);
			}
			fd=fopen(ptr->fn, "a");
			printf("%d, %lf\n", ptr->nodes, ptr->timing);
			fprintf(fd, "%lld %lf\n", ptr->nodes, ptr->timing);
			fclose(fd);
		}
		nodes/=2;
		curf=0;
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

void WriteTiming(TimingTable* tt, int curf, const char* fn, int nodes, double timing)
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
