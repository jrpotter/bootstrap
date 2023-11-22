COMMAND=clang -g -I include src/*.c main.c -o spec

all:
	@${COMMAND}

bear:
	@bear -- ${COMMAND}
