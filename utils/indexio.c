#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <ctype.h>
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

static doc_t *make_doc(int docID, int count) {
    doc_t *docp = (doc_t*) malloc(sizeof(doc_t));
    docp->id = docID;
    docp->count = count;
    return docp;
}

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

hashtable_t *indexload(char *indexnm, char*dirnm){
    char filename[16];
    sprintf(filename, "%s%s", dirnm, indexnm);
    int result = access(filename, F_OK);
    if (result != 0) {
        printf("File does not exist!!\n");
        return NULL;
    }
    FILE * fp;
    fp = fopen(filename, "r");
    hashtable_t *ht = hopen(10);
    char str[1000];
    while ( fgets (str, 1000, fp)!=NULL ) {
        int i, spaces = 0;
        for(i = 0; str[i] != '\0'; i++){
            if (str[i] == ' '){
                spaces++;
            }
        }
        // printf("%d", spaces);

        char *ptr = strtok(str, " ");
        char *w = malloc(sizeof(char *));
        strcpy(w, ptr);
        printf("%s\n",w);
        int j ;
        word_t *wordp = malloc(sizeof(word_t));
        wordp->word = w;
        wordp->qdocument = qopen();

        for ( j = 0 ; j < (spaces-1)/2 ; j++){
                char id[1000], count[1000];
                ptr = strtok(NULL, " "); //ptr is pointing at id
                strcpy(id,ptr);
                printf("%s\n",id);
                ptr = strtok(NULL, " "); //ptr is pointing at count now
                strcpy(count,ptr);
                printf("%s\n",count);
                printf("id is %s\n",id);
                printf("count is %s\n",count);
                qput(wordp->qdocument, make_doc(atoi(id), atoi(count)));
        }
        hput(ht, (void *) wordp, w, strlen(w));
    }

    fclose(fp);
    return ht;
}