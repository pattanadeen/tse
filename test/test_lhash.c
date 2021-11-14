#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <car.h>
#include <lhash.h>
#include <pthread.h>

typedef struct arguments {
    lhashtable_t *lhtp;
    car_t *carp;
    void (*fn)(void* elementp);
} arguments_t;

void fn(void *elementp) {
    car_t *carp = (car_t*) elementp;
    carp->price = 420;
}

bool searchfn(void* elementp,const void* keyp){
    if(keyp == NULL || elementp == NULL){
        printf("NULL value");
        return false;
    }
    if(elementp == keyp){
        return true;
    }
    else{
        return false;
    }
}

void *routine_lhput(void *argp) {
    printf("[Process] Putting car %s\n", ((car_t *) ((arguments_t *) argp)->carp)->plate);
    lhput(
        ((arguments_t *) argp)->lhtp, 
        ((arguments_t *) argp)->carp,
        ((car_t *)(((arguments_t *) argp)->carp))->plate,
        strlen(((car_t *)(((arguments_t *) argp)->carp))->plate)
    );
    printf("[Process] Finish Putting car %s\n", ((car_t *) ((arguments_t *) argp)->carp)->plate);
    return NULL;
}

void *routine_lhapply(void *argp) {
    printf("[Process] Applying function\n");
    lhapply(((arguments_t *) argp)->lhtp, ((arguments_t *) argp)->fn);
    printf("[Process] Finish applying function\n");
    return NULL;
}

void *routine_lhsearch(void *argp) {
    printf("[Process] Searching car %s\n", ((car_t *) ((arguments_t *) argp)->carp)->plate);
    lhsearch(
        ((arguments_t *) argp)->lhtp, 
        searchfn, 
        ((car_t *)(((arguments_t *) argp)->carp))->plate,
        strlen(((car_t *)(((arguments_t *) argp)->carp))->plate)
    );
    printf("[Process] Finish searching car %s\n", ((car_t *) ((arguments_t *) argp)->carp)->plate);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t tid1, tid2, tid3, tid4;
    queue_t *lhtp = lhopen(10);

    printf("-----Single Threaded and Dual-Threaded Tests-----\n");

    printf("\n[Test] Put 1 car\n");

    car_t *car1 = make_car("1gt7117", 2100000, 2018);
    arguments_t *argp1 = (arguments_t *) malloc(sizeof(arguments_t));
    argp1->lhtp = lhtp;
    argp1->carp = car1;
    argp1->fn = fn;

    if(pthread_create(&tid1, NULL, routine_lhput, argp1)!=0) {
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid1, NULL)!=0) {
        exit(EXIT_FAILURE);
    }

    printf("\n[Test] Put 1 car\n");

    car_t *car2 = make_car("5ab2131", 2700000, 2019);
    arguments_t *argp2 = (arguments_t *) malloc(sizeof(arguments_t));
    argp2->lhtp = lhtp;
    argp2->carp = car2;
    argp2->fn = fn;

    if(pthread_create(&tid2, NULL, routine_lhput, argp2)!=0) {
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid2, NULL)!=0) {
        exit(EXIT_FAILURE);
    }

    printf("\n[Test] Put 2 cars\n");

    car_t *car3 = make_car("6ff3145", 1100000, 2015);
    arguments_t *argp3 = (arguments_t *) malloc(sizeof(arguments_t));
    argp3->lhtp = lhtp;
    argp3->carp = car3;
    argp3->fn = fn;

    car_t *car4 = make_car("4pi3142", 900000, 2017);
    arguments_t *argp4 = (arguments_t *) malloc(sizeof(arguments_t));
    argp4->lhtp = lhtp;
    argp4->carp = car4;
    argp4->fn = fn;
    
    if(pthread_create(&tid3, NULL, routine_lhput, argp3)!=0) {
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&tid4, NULL, routine_lhput, argp4)!=0) {
        exit(EXIT_FAILURE);
    }  

    if(pthread_join(tid3, NULL)!=0) {
        exit(EXIT_FAILURE);
    }
    if(pthread_join(tid4, NULL)!=0) {
        exit(EXIT_FAILURE);
    }

    printf("\n[Test] Apply a function\n");

    if(pthread_create(&tid1, NULL, routine_lhapply, argp1)!=0) {
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid1, NULL)!=0) {
        exit(EXIT_FAILURE);
    }

    printf("\n[Test] Search 2 cars\n");

    if(pthread_create(&tid2, NULL, routine_lhsearch, argp1)!=0) {
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&tid4, NULL, routine_lhsearch, argp2)!=0) {
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid2, NULL)!=0) {
        exit(EXIT_FAILURE);
    }
    if(pthread_join(tid4, NULL)!=0) {
        exit(EXIT_FAILURE);
    }

    lhclose(lhtp);
    free(argp1);
    free(argp2);
    free(argp3);
    free(argp4);
}