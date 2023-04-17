all:
	gcc -o main main.c BinTree.c funcs.c menue.c 
tree:
	gcc -c BinTree.c funcs.c -fpic 
	gcc BinTree.o funcs.o -shared -o libbintree.so
	gcc -c menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c BinTree.c menue.c
	gcc -o main main.o -L. -lbintree -lmenue -lm
	mv BinTree.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
