all:
	gcc -o main main.c BinTree.c funcs.c menue.c 
tree:
	gcc -c -g Btree.c funcs.c timer.c -fpic 
	gcc Btree.o funcs.o timer.o -shared -o libbtree.so
	gcc -c -g menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c -g main.c funcs.c Btree.c menue.c timer.c
	gcc -o -g main main.o -L. -lbtree -lmenue
	mv Btree.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
	mv timer.o binaries
itr-tree:
	gcc -c Btree.c funcs.c timer_it.c -fpic 
	gcc Btree.o funcs.o timer_it.o -shared -o libbtree.so
	gcc -c menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c Btree.c menue.c timer_it.c
	gcc -o main main.o -L. -lbtree -lmenue
	mv Btree.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
	mv timer_it.o binaries
