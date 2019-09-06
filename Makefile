all:
	rm query-cmus
	gcc query-cmus.c -o query-cmus
	./query-cmus
