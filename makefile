all:	thorn

thorn:	thorn.c thorn.h
	gcc -o thorn thorn.c

clean:
	rm -f *.o thorn

