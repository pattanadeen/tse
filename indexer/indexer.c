#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "pageio.h"
#include "webpage.h"
#include "hash.h"

typedef struct word {
	char *word;
	int count;
} word_t;

bool searchfn(void* elementp, const void* keyp){
    if(keyp == NULL || elementp == NULL){
        printf("NULL value");
        return false;
    }
    
    if(strcmp(webpage_getURL(elementp), (char *)keyp) == 0) {
        return true;
    }
    else{
        return false;
    }
}

void SumHash(void *elementp){
    word_t *wordp = (word_t*) elementp;
    printf("%d\n", wordp->count);
}

void free_word(void *elementp){
    word_t *wordp = (word_t*) elementp;
    free(wordp->word);
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
    webpage_t *pagep = pageload(1, "../pages-depth3/");
    char *word;
    int pos = 0;
    hashtable_t *htp = hopen(10);

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
    
    happy(htp, SumHash);

    webpage_delete(pagep);
    free(pagep);
    free(word);

    happly(htp, free_word);
    hclose(htp);

    return 0;
}