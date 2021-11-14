#pragma once
/* 
 * lqueue.h -- public interface to the lqueue module
 */
#include <stdint.h>
#include <stdbool.h>

/* the lqueue representation is hidden from users of the module */
typedef struct lqueue lqueue_t;		

/* create an empty lqueue */
lqueue_t* lqopen(void);        

/* deallocate a lqueue, frees everything in it */
void lqclose(lqueue_t *lqp);   

/* put element at the end of the lqueue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(lqueue_t *lqp, void *elementp); 

/* get the first first element from lqueue, removing it from the lqueue */
void* lqget(lqueue_t *lqp);

/* apply a function to every element of the lqueue */
void lqapply(lqueue_t *lqp, void (*fn)(void* elementp));

/* search a lqueue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the lqueue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* lqsearch(lqueue_t *lqp, 
							bool (*searchfn)(void* elementp,const void* keyp),
							const void* skeyp);

/* search a lqueue using a supplied boolean function (as in qsearch),
 * removes the element from the lqueue and returns a pointer to it or
 * NULL if not found
 */
// void* lqremove(lqueue_t *lqp,
// 							bool (*searchfn)(void* elementp,const void* keyp),
// 							const void* skeyp);

// /* concatenatenates elements of q2 into q1
//  * q2 is dealocated, closed, and unusable upon completion 
//  */
// void lqconcat(lqueue_t *q1p, lqueue_t *q2p);

