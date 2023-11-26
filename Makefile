# ============================================================
# Configuration
# ============================================================

# To create a release build, run `make BUILD=release`.

BUILD := debug
PREFIX := ${CURDIR}/dist/${BUILD}
OUT := bootstrap

CCFLAGS.debug := -DDEBUG -g -Og
CCFLAGS.release := -DNDEBUG
LDFLAGS := -lm

# ============================================================
# Build
# ============================================================

COMPILE := ${CC} ${CCFLAGS.${BUILD}} -I include src/*.c main.c -o ${PREFIX}/${OUT} ${LDFLAGS}

all: build all.${BUILD}
all.debug: bear
all.release:

build: ${PREFIX}/${OUT}

${PREFIX}/${OUT}: ${PREFIX} include/*.h src/*.c
	${COMPILE}

${PREFIX}:
	mkdir -p $@

# ============================================================
# Compilation Database
#
# Generate a compilation database using [Bear](https://github.com/rizsotto/Bear).
# ============================================================

bear: compile_commands.json

compile_commands.json: include/*.h src/*.c main.c
# This file is only constructed in debug mode. If interested in expanding this
# generation to other build types, add a release-specific dependency.
ifeq ($(BUILD), debug)
	mkdir -p dist/debug
	bear -- ${COMPILE}
endif

# ============================================================
# Documentation.
#
# Generate documentation using [Doxygen](https://www.doxygen.nl/index.html).
# ============================================================

docs: docs/index.html

# The `index.html` file is regenerated on each invocation of `doxygen`.
docs/index.html: Doxyfile include/*.h src/*.c
	doxygen

# ============================================================
# Testing.
#
# We use [Sput](https://www.use-strict.de/sput-unit-testing/) to run tests.
# ============================================================

test: dist/test/suites
	dist/test/suites

dist/test/suites: include/*.h src/*.c test/*.h test/*.c
	mkdir -p dist/test
	${CC} ${CCFLAGS.debug} -I include src/*.c test/*.c -o dist/test/suites ${LDFLAGS}

# ============================================================
# Other
# ============================================================

clean:
	rm -r ${PREFIX}

.PHONY: test
