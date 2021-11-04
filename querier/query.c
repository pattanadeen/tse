#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <indexio.h>
#include <queue.h>

typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;

typedef struct doc {
	int id;
	int count;
} doc_t;

static bool search_word(void* elementp, const void* keyp){
    if(keyp == NULL || elementp == NULL){
        printf("NULL value");
        return false;
    }

    if(strcmp(((word_t *) elementp)->word, (char *)keyp) == 0) {
        return true;
    }
    else{
        return false;
    }
}

static bool search_doc(void* elementp, const void* keyp){
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

static void strip_extra_spaces(char* str) {
    int i, x;

    for(i=x=0; str[i]; ++i) {
        if(!isspace(str[i]) || (i > 0 && !isspace(str[i-1]))) {
            str[x++] = str[i];
        }  
    }

    str[x-1] = '\0';
}


static int normalize_sentence(char *str) {
    if (str[0] == '\n') {
        return 0;
    }
    for (size_t i = 0; i < strlen(str); ++i) {
        if (isalpha(str[i]) != 0 || isspace(str[i])) {
            str[i] = tolower((unsigned char) str[i]);
        }
        else {
            return -1;
        }
    }

    strip_extra_spaces(str);

    return 0;
}

/* Step 1
int main(int argc, char *argv[]) {
    while (1) {
        char *str = (char*) malloc(sizeof(char));
        char cha;
        int i = 1;

        while (1) {
            if (scanf("%c", &cha) == -1) {
                free(str);
                exit(EXIT_SUCCESS);
            }

            i++;
            str = (char *) realloc(str, sizeof(char) * i);
            str[i-2] = cha;
            str[i-1] = '\0';

            if (cha == '\n') {
                break;
            }            
        }

        if (normalize_sentence(str) == -1) {
            printf("[invalid query]\n");
        }
        else {
            printf("%s\n", str);
        }
        
        free(str);
    }
}
*/


int main(int argc, char *argv[]) {
    char *str = (char*) malloc(sizeof(char));
    char cha;
    int i = 1;

    while (1) {
        if (scanf("%c", &cha) == -1) {
            free(str);
            exit(EXIT_SUCCESS);
        }

        i++;
        str = (char *) realloc(str, sizeof(char) * i);
        str[i-2] = cha;
        str[i-1] = '\0';

        if (cha == '\n') {
            break;
        }            
    }

    if (normalize_sentence(str) == -1) {
        printf("[invalid query]\n");
        free(str);
        exit(EXIT_FAILURE);
    }    

    int id = 1, rank = 0;
    hashtable_t *htp = indexload("indexnmq", "../indices/");

    const char s[2] = " ";
    char *word = strtok(str, s);
    while(word != NULL) {
        word_t *wordp = hsearch(htp, search_word, word, strlen(word));

        if (wordp == NULL) {
            word = strtok(NULL, s);
            continue;
        }

        doc_t *docp = qsearch(wordp->qdocument, search_doc, &id);

        if (docp == NULL) {
            word = strtok(NULL, s);
            continue;
        }

        if (rank == 0 || docp->count < rank) {
            rank = docp->count;
        }
        // printf("%s\n", word);

        printf( "%s:%d ", word, docp->count);
        word = strtok(NULL, s);
    }

    printf("-- %d\n", rank);
    
    free(str);
}
