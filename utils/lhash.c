#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "hash.h"
#include "lhash.h"

typedef struct lhashtable {
 	hashtable_t *htp;
    pthread_mutex_t m;
} lhashtable_t;

lhashtable_t *lhopen(uint32_t hsize) {
    lhashtable_t *lhtp = (lhashtable_t *) malloc(sizeof(lhashtable_t));
    pthread_mutex_init(&lhtp->m, NULL);
    lhtp->htp = hopen(hsize);
    return lhtp;
}

void lhclose(lhashtable_t *lhtp) {
    hclose(lhtp->htp);
    pthread_mutex_destroy(&lhtp->m);
    free(lhtp);
}

int32_t lhput(lhashtable_t *lhtp, void *ep, const char *key, int keylen) {
    pthread_mutex_lock(&lhtp->m);
    int32_t result = hput(lhtp->htp, ep, key, keylen);
    sleep(1);
    pthread_mutex_unlock(&lhtp->m);
    return result;
}

void lhapply(lhashtable_t *lhtp, void (*fn)(void* elementp)) {
    pthread_mutex_lock(&lhtp->m);
    happly(lhtp->htp, fn);
    sleep(1);
    pthread_mutex_unlock(&lhtp->m);
}

void *lhsearch(lhashtable_t *lhtp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen) {
    pthread_mutex_lock(&lhtp->m);
    void* result = hsearch(lhtp->htp, searchfn, key, keylen);
    sleep(1);
    pthread_mutex_unlock(&lhtp->m);
    return result;
}