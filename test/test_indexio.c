#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "indexio.h"
#include "pageio.h"
#include "webpage.h"
#include "queue.h"
#include "hash.h"

typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;

void free_word(void *elementp){
    word_t *wordp = (word_t*) elementp;

    free(wordp->word);
    qclose(wordp->qdocument);
}

int main(int argc, char *argv[]) {

    hashtable_t *ht = indexload("indexnm", "../indices/");
    indexsave(ht, "indexnm_test", "../indices/");

    happly(ht, free_word);
    hclose(ht);

    exit(EXIT_SUCCESS);

}