#ifndef TIMER_H
#define TIMER_H

typedef struct Item
{
	const char* fn; //file name where results will be stored
	long long nodes; //amount of nodes tested
	long double timing; //An average time result
}Item;

typedef struct TimingTable
{
	int msize;
	int csize;
	Item* info;
}TimingTable;

void generate(Node* root, int n, long long int limit, long long int str_limit);
void timing(long long nodes, long long limit, long long str_limit, int itr);
Item* Searchtt(TimingTable* vector, int ms, char* fn);
void WriteTiming(TimingTable* tt, int curf, const char* fn, int nodes, long double timing);
int Traversing_no_print(Node* root, int key);

#endif
