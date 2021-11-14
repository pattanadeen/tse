#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"
#include "lqueue.h"

typedef struct lqueue {
 	queue_t *qp;
    pthread_mutex_t m;
} lqueue_t;

lqueue_t* lqopen(void) {
    lqueue_t *lqp = (lqueue_t *) malloc(sizeof(lqueue_t));
    pthread_mutex_init(&lqp->m, NULL);
    lqp->qp = qopen();
    return lqp;
}

void lqclose(lqueue_t *lqp) {
    qclose(lqp->qp);
    pthread_mutex_destroy(&lqp->m);
    free(lqp);
}

int32_t lqput(lqueue_t *lqp, void *elementp) {
    pthread_mutex_lock(&lqp->m);
    int32_t result = qput(lqp->qp, elementp);
    sleep(1);
    pthread_mutex_unlock(&lqp->m);
    return result;
}

void* lqget(lqueue_t *lqp) {
    pthread_mutex_lock(&lqp->m);
    void* result = qget(lqp->qp);
    sleep(1);
    pthread_mutex_unlock(&lqp->m);
    return result;
}

void lqapply(lqueue_t *lqp, void (*fn)(void* elementp)) {
    pthread_mutex_lock(&lqp->m);
    qapply(lqp->qp, fn);
    sleep(1);
    pthread_mutex_unlock(&lqp->m);
}

void* lqsearch(lqueue_t *lqp, bool (*searchfn)(void* elementp,const void* keyp), const void* skeyp) {
    pthread_mutex_lock(&lqp->m);
    void* result = qsearch(lqp->qp, searchfn, skeyp);
    sleep(1);
    pthread_mutex_unlock(&lqp->m);
    return result;
}