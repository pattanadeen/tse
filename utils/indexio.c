#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "indexio.h"
#include "hash.h"
#include "queue.h"

char *string;
int i;

typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;

typedef struct doc {
	int id;
	int count;
} doc_t;

void save_doc(void *elementp) {
    doc_t *docp = (doc_t *) elementp;
    sprintf(string, "%d %d ", docp->id, docp->count);
}

void save_word(void *elementp) {
    word_t *wordp = (word_t*) elementp;
    string = (char *) realloc(string, sizeof(char) * 1000000);
    sprintf(string, "%s%s ", string, wordp->word);
    i = i + strlen(wordp->word);
    string[i-1] = '\0';
    // exit(EXIT_SUCCESS);

    // // qapply(wordp->qdocument, save_doc);
    // sprintf(string, "%s\n", string);
}

int32_t indexsave(hashtable_t *htp, char *indexnm, char *dirnm) {
    char filename[strlen(dirnm)];
    sprintf(filename, "%s%s", dirnm, indexnm);
    // int result = access(filename, F_OK);

    // if (result == 0) {
    //     printf("File already exist in the directory!!\n");
    //     return -1;
    // }

    FILE * fp;
    fp = fopen(filename, "w");

    string = (char*) calloc(1, sizeof(char));
    i = 0;
    happly(htp, save_word);

    fprintf(fp, "%s", string);

    fclose(fp);
    free(string);

    return 0;
}