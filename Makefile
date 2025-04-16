CC=gcc
CFLAGS=-Wall -std=c99

help:
	@echo "If you want to compile all binaries, use 'make all' "
	@echo "Else use 'make <binary>'"

swc: src/swc.c bin
	$(CC) $(CFLAGS) -o bin/swc src/swc.c

all: swc

bin:
	mkdir bin/

clean: bin
	rm -r bin/
