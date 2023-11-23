BUILD=clang -Og -g -I include src/*.c main.c -o spec

all: build bear

build: include/*.h src/*.c
	@${BUILD}

bear: include/*.h src/*.c
	@bear -- ${BUILD}

test: test/runner
	$^

test/runner: include/*.h src/*.c test/*.h test/*.c
	clang -I include src/*.c test/*.c -o test/runner

.PHONY: test
