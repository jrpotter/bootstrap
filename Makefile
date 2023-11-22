COMMAND=clang -g -I include src/*.c main.c -o gen-flake

all:
	@${COMMAND}

bear:
	@bear -- ${COMMAND}
