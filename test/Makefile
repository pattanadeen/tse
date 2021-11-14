CFLAGS=-Wall -pedantic -std=c11 -I ../utils/ -L ../lib/ -g
LIBS=-lutils -lcurl -lpthread 

all:			test_pageio test_indexio test_lqueue

test_pageio:	test_pageio.c 
				gcc $(CFLAGS) test_pageio.c $(LIBS) -o $@

test_indexio:	test_indexio.c 
				gcc $(CFLAGS) test_indexio.c $(LIBS) -o $@

test_lqueue:	test_lqueue.c
				gcc $(CFLAGS) test_lqueue.c $(LIBS) -o $@

clean:
				rm -f *.o test_pageio test_indexio test_lqueue