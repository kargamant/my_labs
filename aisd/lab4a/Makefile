all:
	gcc -o main main.c BinTree.c funcs.c menue.c 
tree:
	gcc -c BinTree.c funcs.c timer.c -fpic 
	gcc BinTree.o funcs.o timer.o -shared -o libbintree.so
	gcc -c menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c BinTree.c menue.c timer.c
	gcc -o main main.o -L. -lbintree -lmenue
	mv BinTree.o binaries
	mv timer.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
