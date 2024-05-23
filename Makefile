CC=gcc
CFLAGS=-I/usr/local/include  # 添加 CUnit 头文件的路径
LDFLAGS=-L/usr/local/lib -lcunit  # 添加 CUnit 库的路径和链接库

all: build_test

build_test:
	$(CC) -o test test.c collection.c $(CFLAGS) $(LDFLAGS)
