#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "pageio.h"
#include "webpage.h"
#include "hash.h"
#include "queue.h"
#include "indexio.h"

int sum;

/*
 *  Step 3
 *
typedef struct word {
	char *word;
	int count;
} word_t;
 *
 *  Step 3
 */

typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;

typedef struct doc {
	int id;
	int count;
} doc_t;

doc_t *make_doc(int docID, int count) {
    doc_t *docp = (doc_t*) malloc(sizeof(doc_t));
    docp->id = docID;
    docp->count = count;
    return docp;
}

bool search_word(void* elementp, const void* keyp) {
    if(keyp == NULL || elementp == NULL){
        printf("NULL value");
        return false;
    }

    if(strcmp(((word_t *) elementp)->word, (char *) keyp) == 0) {
        return true;
    }
    else{
        return false;
    }
}

bool search_doc(void* elementp, const void* keyp) {
    if(keyp == NULL || elementp == NULL){
        printf("NULL value");
        return false;
    }
    
    if(((doc_t *) elementp)->id == *((int *) keyp)) {
        return true;
    }
    else{
        return false;
    }
}

/*
 *  Step 3
 *
void sumwords(void *elementp){
    word_t *wordp = (word_t*) elementp;
    sum = sum + wordp->count;
}
 *
 *  Step 3
 */

void qsumwords(void *elementp) {
    doc_t *docp = (doc_t*) elementp;
    sum = sum + docp->count;
}

void sumwords(void *elementp){
    word_t *wordp = (word_t*) elementp;
    qapply(wordp->qdocument, qsumwords);
}

void free_word(void *elementp){
    word_t *wordp = (word_t*) elementp;

    free(wordp->word);
    qclose(wordp->qdocument);
}

int NormalizeWord(char *word){
    size_t length = strlen(word);  
    
    if(length < 3){
        return -1;
    }

    for (size_t i = 0; i < strlen(word); ++i) {
        if (isalpha(word[i]) != 0) {
            word[i] = tolower((unsigned char) word[i]);
        }
        else {
            return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int id = atoi(argv[1]);
    
    hashtable_t *htp = hopen(10);

    /*
     *  Step 3
     *
    while ((pos = webpage_getNextWord(pagep, pos, &word)) > 0){
        if(NormalizeWord(word) == 0){
            // printf("%s\n", word);
            word_t *words = hsearch(htp, searchfn, word, strlen(word));

            if(words != NULL){
                words->count = words->count +1;
                free(word);
            }
            else{
                word_t *wordp = malloc(sizeof(word_t));
                wordp->word = word;
                wordp->count = 1;
                hput(htp, (void *) wordp, word, strlen(word));
            }
        } else {
            free(word);
        }
    }
    
    sum = 0;
    happly(htp, sumwords);
    printf("%d", sum);
     *
     *  Step 3
     */

    int i;
    for (i = 1; i <= id; i++) {
        webpage_t *pagep = pageload(i, "../pages-depth3/");
        char *word;
        int pos = 0;

        while ((pos = webpage_getNextWord(pagep, pos, &word)) > 0){
            if(NormalizeWord(word) == 0){
                word_t *words = hsearch(htp, search_word, word, strlen(word));
                doc_t *document;
                if(words != NULL){
                    if ((document = (doc_t *) qsearch(words->qdocument, search_doc, &i)) != NULL) {
                        document->count = document->count + 1;
                    }
                    else {
                        qput(words->qdocument, make_doc(i, 1));
                    }
                }
                else{
                    word_t *wordp = malloc(sizeof(word_t));
                    wordp->word = word;
                    wordp->qdocument = qopen();

                    qput(wordp->qdocument, make_doc(i, 1));
                    hput(htp, (void *) wordp, word, strlen(word));
                }
            } else {
                free(word);
            }
        }

        webpage_delete(pagep);
        free(pagep);
        free(word);
    }    
    
    // sum = 0;
    // happly(htp, sumwords);
    // printf("%d", sum);

    indexsave(htp, "indexnm", "../indices/");

    happly(htp, free_word);
    hclose(htp);

    exit(EXIT_SUCCESS);
}