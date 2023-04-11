all:
	gcc -o test test.c HashLib/HashTable.c HashLib/Hash.c -lm
hash:
	gcc -c HashLib/HashTable.c HashLib/Hash.c funcs_hash.c -fpic 
	gcc HashTable.o funcs_hash.o Hash.o -shared -o libhtable.so
	gcc -c menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcs_hash.c HashLib/HashTable.c menue.c HashLib/Hash.c
	gcc -o main main.o -L. -lhtable -lmenue -lm
	mv HashTable.o binaries
	mv funcs_hash.o binaries
	mv main.o binaries
	mv menue.o binaries
	mv Hash.o binaries
