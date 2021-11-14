CFLAGS=-Wall -pedantic -std=c11 -I ../utils/ -L ../lib/ -g
LIBS=-lutils -lcurl -lpthread

all:			indexer

indexer:		indexer.c
				gcc $(CFLAGS) indexer.c $(LIBS) -o $@

clean:
				rm -f *.o indexer
			