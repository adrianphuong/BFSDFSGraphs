CC = clang
CFLAGS = -g -Wall -Wpedantic -Werror -Wextra


all: main

main: main.o graphsearch.o
	$(CC) graphsearch.o main.o -o main

graphsearch.o: graphsearch.c graphsearch.h
	$(CC) $(CFLAGS) -c graphsearch.c

main.o: main.c graphsearch.h
	$(CC) $(CFLAGS) -c main.c

graph1.dot.png: graph1.dot
	dot graph1.dot -O -Tpng

graph2.dot.png: graph2.dot
	dot graph2.dot -O -Tpng

clean:
	rm -f main main.o graphsearch.o

format:
	clang-format -i -style=file *.{c,h}
