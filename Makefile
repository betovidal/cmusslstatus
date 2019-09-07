all:
	rm query-cmus || true
	gcc query-cmus.c -o query-cmus -std=c99 -pedantic -Wall -Wextra -Os
	./query-cmus
