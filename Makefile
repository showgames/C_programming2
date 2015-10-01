#Makefile
main: main.c game.c think.c option.c game.h
		gcc -Wall -o main main.c game.c think.c option.c
