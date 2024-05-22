all: build_test

build_test:
	gcc -o test test.c collection.c
