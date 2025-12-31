CC=gcc
CFLAGS=-Wall -std=c99 -g

help:
	@echo "If you want to compile all binaries, use 'make all' "
	@echo "Else use 'make <binary>'"

swc: src/swc.c bin
	$(CC) $(CFLAGS) -o bin/swc src/swc.c

scp: src/scp/scp.cpp bin
	g++ -Wall -Wextra -std=c++11 -lc -g -o bin/scp src/scp/scp.cpp

all: swc

bin:
	mkdir bin/

clean: bin
	rm -r bin/
