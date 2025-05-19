all: main

main: functest.o main.c
	gcc main.c functest.o -o main

functest.o: functest.c functest.h
	gcc -c functest.c

run: main
	./main