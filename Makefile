CC=gcc
CFLAGS=-Wall -std=c99 -g

help:
	@echo "If you want to compile all binaries, use 'make all' "
	@echo "Else use 'make <binary>'"

swc: src/swc.c bin
	$(CC) $(CFLAGS) -o bin/swc src/swc.c

scp: src/scp/scp.cpp src/scp/filedescriptor.cpp src/scp/copy.cpp bin
	g++ -Wall -Wextra -std=c++11 -lc -g -c src/scp/scp.cpp -o obj/scp-scp.out
	g++ -Wall -Wextra -std=c++11 -lc -g -c src/scp/filedescriptor.cpp -o obj/scp-filedescriptor.out
	g++ -Wall -Wextra -std=c++11 -lc -g -c src/scp/copy.cpp -o obj/scp-copy.o
	g++ obj/*.o -o bin/scp

all: swc scp

bin:
	mkdir bin/

obj:
	mkdir obj/

clean: bin obj
	rm -r bin/
