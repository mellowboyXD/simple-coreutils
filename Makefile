CC=gcc
CFLAGS=-Wall

help:
	@echo "If you want to compile all binaries, use 'make all' "
	@echo "Else use 'make <binary>'"

swc: src/swc.c
	$(CC) $(CFLAGS) -o bin/swc swc.c

all: swc
