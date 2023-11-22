all:
	@clang -g -lncurses -I include src/*.c main.c -o gen-flake

bear:
	@bear -- clang -g -lncurses -I include src/*.c main.c -o gen-flake
