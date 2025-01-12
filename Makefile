all: cc c0
	@ echo 'done'

cc: cc.c cc.h
	gcc -std=c2x -ggdb cc.c -o cc

c0: c00.c
	gcc -std=c2x -ggdb c00.c -o c0