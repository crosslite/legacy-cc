CFLAGS = -std=c2x -ggdb

all: cc c0 c1
	@ echo 'done'

cc: cc.c cc.h
	gcc $(CFLAGS) -o cc cc.c

c0: c/c00.c
	gcc $(CFLAGS) -o c0 c/c00.c

c1: c/c10.c
	gcc $(CFLAGS) -o c1 c/c10.c