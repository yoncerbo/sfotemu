RELEASE_FLAGS = -O2
DEV_FLAGS = -O0 -g3
TEST_FLAGS = -O0 -g3
CFLAGS = -std=c99 -Wall -Wextra -I ./src/headers -I ./src

.PHONY: run

build-release: src/*
	mkdir -p out
	gcc ${CFLAGS} ${RELEASE_FLAGS} -o out/release src/main.c

build-dev: src/*
	mkdir -p out
	gcc ${CFLAGS} ${DEV_FLAGS} -o out/dev src/main.c

release: build-release
	./out/release $(file)

run: build-dev
	./out/dev $(file)

out/test_%: src/test_%.c src/*
	mkdir -p out
	gcc ${CFLAGS} ${TEST_FLAGS} -o $@ $<

test_%: out/test_%
	$<
