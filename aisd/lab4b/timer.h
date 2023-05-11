#ifndef TIMER_H
#define TIMER_H

typedef struct Row
{
	const char* fn; //file name where results will be stored
	long long nodes; //amount of nodes tested
	long double timing; //An average time result
}Row;

typedef struct TimingTable
{
	int msize;
	int csize;
	Row* info;
}TimingTable;

void generate(Btree* tr, int n, long long int limit, long long int str_limit);
void timing(long long nodes, long long limit, long long str_limit, int itr, double step);
Row* Searchtt(TimingTable* vector, int ms, char* fn);
void WriteTiming(TimingTable* tt, int curf, const char* fn, int nodes, long double timing);
int Traversing_no_print(Btree* tr, int key);

#endif
