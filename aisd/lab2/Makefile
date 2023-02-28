all:

vector:
	gcc -c stack_lib/stack_v.c funcs.c -fpic
	gcc stack_v.o funcs.o -shared -o libstack.so
	gcc -c prog.c -o prog.o
	gcc -o prog prog.o -L. -lstack
	mv stack_v.o binaries
	mv funcs.o binaries
	mv prog.o binaries	
list:
	gcc -c stack_lib/stack_l.c stack_lib/list.c funcs.c -fpic
	gcc stack_l.o list.o funcs.o -shared -o libstack.so
	gcc -c prog.c -o prog.o
	gcc -o prog prog.o -L. -lstack
	mv stack_l.o binaries
	mv list.o binaries
	mv funcs.o binaries
	mv prog.o binaries
