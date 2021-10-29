#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "indexio.h"
#include "hash.h"
#include "queue.h"

char *string;

typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;

typedef struct doc {
	int id;
	int count;
} doc_t;

static void save_doc(void *elementp) {
    doc_t *docp = (doc_t *) elementp;
    char id[10], count[10];

    sprintf(id, "%d ", docp->id);
    sprintf(count, "%d ", docp->count);

    string = (char *) realloc(string, sizeof(char) * (strlen(string) + strlen(id) + strlen(count) + 1));
    strcat(string, id);
    strcat(string, count);
}

static void save_word(void *elementp) {
    word_t *wordp = (word_t*) elementp;

    string = (char *) realloc(string, sizeof(char) * (strlen(string) + strlen(wordp->word) + 2));
    strcat(strcat(string, wordp->word), " ");

    qapply(wordp->qdocument, save_doc);

    string = (char *) realloc(string, sizeof(char) * (strlen(string) + 2));
    strcat(string, "\n");
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
    happly(htp, save_word);
    
    size_t ln = strlen(string) - 1;
    if (*string && string[ln] == '\n') 
    string[ln] = '\0';
    
    fprintf(fp, "%s", string);

    fclose(fp);
    free(string);

    return 0;
}