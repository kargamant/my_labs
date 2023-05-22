graph:
	gcc -c -g Graph.c funcs.c Queue.c List.c HashLib/HashTable.c HashLib/Hash.c Heap.c -fpic 
	gcc Graph.o funcs.o Queue.o List.o HashTable.o Hash.o Heap.o -shared -o libgraph.so
	gcc -c -g main.c funcs.c Graph.c Queue.c List.c menue.c HashLib/HashTable.c HashLib/Hash.c Heap.c
	gcc -o main main.o -L. -lgraph -lmenue -lm
	mv Graph.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv Queue.o binaries
	mv List.o binaries
	mv HashTable.o binaries
	mv Hash.o binaries
	mv Heap.o binaries
