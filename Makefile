BUILD=clang -Og -g -I include src/*.c main.c -o bootstrap -lm

all: build bear

build: include/*.h src/*.c
	@${BUILD}

bear: include/*.h src/*.c
	@bear -- ${BUILD}

test: test/test
	$^

test/test: include/*.h src/*.c test/*.h test/*.c
	clang -I include src/*.c test/*.c -o test/test -lm

.PHONY: test
