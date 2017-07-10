# Makefile
.PHONY: cast # I am not sure how to fix this and am lazy
LDFLAGS=-lncurses

all: cast

cast:
	gcc -o cast -lcurses -lm cast.c player.c

clean:
	rm cast

docker:
	docker build --tag dungeon-test .
