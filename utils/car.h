#include <stdint.h>
#include "queue.h"
#define MAXREG 10

typedef struct car {
	char plate[MAXREG];
	double price;
	int year;
} car_t;

typedef struct node {
 	struct node *next;
 	void *element;
} node_t;

typedef struct queue {
 	struct node *front;
    struct node *back;
} queue_s;

typedef struct hashtable {
	uint32_t hsize;
	queue_s **htable;
} hashtable_s;

car_t *make_car(char *platep, double price, int year);

void print_car(car_t *carp);

void print_car_queue(queue_s *qp);

void print_car_hash(hashtable_s *htp);