# Makefile
LDFLAGS=-lncurses

all: cast

cast:
	gcc -o cast -l curses cast.c player.c
