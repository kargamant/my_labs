#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"
#include "funcs.h"
#include <time.h>
#include "timer.h" 
#include <string.h>

void generate(Btree* tr, int n, long long limit, long long str_limit)
{
	srand(time(NULL));
	for(int i=0; i<n; i++)
	{
		int key=rand()%limit;
		int size=rand()%str_limit;
		while(size==0) size=3+rand()%str_limit;
		char* info=malloc(size*sizeof(char));
		info[size-1]=0;
		for(char* ptr=info; ptr-info<size-1; ++ptr) *ptr=65+32*(rand()%2)+rand()%27;
		AddNode(tr, key, info);
		/*while(result!=ERR_OK)
		{
			key=rand()%limit;
			result=AddNode(root, key, info);
		}*/
	}
}

void timing(long long nodes, long long limit, long long str_limit, int itr, double step)
{
	system("mkdir timing");
	printf("\n");
	const char* result_files[]=
	{
		"timing/AddNode.txt",
		"timing/Traversing.txt",
		"timing/DelNode.txt",
		"timing/Search.txt",
		//"timing/Max.txt",
	};
	
	
	TimingTable* tt=(TimingTable*)malloc(sizeof(TimingTable));
	tt->msize=4;
	tt->csize=0;
	tt->info=(Row*)calloc(tt->msize, tt->msize*sizeof(Row));

	
	for(Row* ptr=tt->info; ptr-tt->info<tt->msize; ++ptr) 
	{
		ptr->fn=result_files[ptr-tt->info];
	}
	
//	if(nodes>limit)
//	{
//		printf("Warning. Key limit is less than amount of nodes. That means that it isn't possible to generate such tree.\n");
//		printf("Key limit was increased to value=nodes*2\n");
//		limit=nodes*2;
//	}
	long long fnodes=nodes;
	long double trave_t=0, max_t=0, search_t=0, add_t=0, del_t=0; 
	srand(time(NULL));
	Btree* tr=NULL;
	//Node* root=(Node*)calloc(1, sizeof(Node));
	while(nodes>10)
	{
		tr=InitBtree(2);
		generate(tr, nodes, limit, str_limit);
		long double t1=0, t2=0;

		printf("nodes: %lld\n", nodes);
		for(int i=0; i<itr; i++)
		{
			//Testing of AddNode
			int key=rand()%limit;
			int size=3+rand()%str_limit;
			char* info=(char*)calloc(size, size*sizeof(char));
			for(char* ptr=info; ptr-info<size-1; ++ptr) *ptr=65+32*(rand()%2)+rand()%27;
			t1=clock();
			AddNode(tr, key, info);
			t2=clock();
			add_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);

			//Testing of Traversing
			t1=clock();
			Traversing_no_print(tr, limit+1);
			t2=clock();
			trave_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);
			

			//Testing of Delition
			//key=rand()%limit;
			t1=clock();
			DelNode(tr, key, 1);
			t2=clock();
			del_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);

			//Testing of search
			//key=rand()%limit;
			t1=clock();
			Search(tr, key);
			t2=clock();
			search_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);

			/*//Testing max
			t1=clock();
			Max(root);
			t2=clock();
			max_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);*/
		}
	//	for(int i=0; i<itr; i++)
	//	{
	//		int key=rand()%limit;
	//		t1=clock();
	//		DelNode(tr, key, 1);
	//		t2=clock();
	//		del_t+=(long double)((t2-t1)/CLOCKS_PER_SEC);
	//	}
		add_t/=itr;
		trave_t/=itr;
		del_t/=itr;
		search_t/=itr;
		//max_t/=itr;
		WriteTiming(tt, 0, result_files[0], nodes, add_t);	
		WriteTiming(tt, 1, result_files[1], nodes, trave_t);	
		WriteTiming(tt, 2, result_files[2], nodes, del_t);	
		WriteTiming(tt, 3, result_files[3], nodes, search_t);	
		//WriteTiming(tt, 4, result_files[4], nodes, max_t);	

		for(Row* ptr=tt->info; ptr-tt->info<tt->msize; ++ptr) 
		{
			FILE* fd=fopen(ptr->fn, "a");
			if(nodes==fnodes)
			{
				fclose(fd);
				fd=fopen(ptr->fn, "w+");
			}
			fprintf(fd, "%lld %.20Lf\n", ptr->nodes, ptr->timing);
			fclose(fd);
		}
		nodes/=step;
		//erase(tr);
		//free(tr);
		/*Node* start=Max(root);
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
		root->info=NULL;*/
	}
	free(tt->info);
	free(tt);
}

Row* Searchtt(TimingTable* vector, int ms, char* fn)
{
	for(Row* ptr=vector->info; ptr-vector->info<ms; ++ptr)
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


int Traversing_no_print(Btree* tr, int key)
{
	Node* ptr=tr->root;
	if(!ptr) return 0;
	for(int i=0; i<ptr->n; i++)
	{
		//if(!ptr->info) continue;
		Btree* ntr=(Btree*)malloc(sizeof(Btree));
		ntr->t=tr->t;
		ntr->root=ptr->child[i];
		Traversing_no_print(ntr, key);
		free(ntr);
		if(ptr->keys[i]<=key)
		{
			//printf("key: %d | info: ", ptr->keys[i]);
			Item* gr=ptr->info + i;
			while(gr)
			{
				//printf("\"%s\", ", gr->data);
				gr=gr->next;
			}
			//printf("\n");
			//printf("info: %s\n", ptr->info[i]);
		}
	}
	Btree* ntr=(Btree*)malloc(sizeof(Btree));
	ntr->t=tr->t;
	ntr->root=ptr->child[ptr->n];
	Traversing_no_print(ntr, key);
	free(ntr);
	return 0;
}
