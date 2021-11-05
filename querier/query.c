#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <indexio.h>
#include <queue.h>
#include <webpage.h>
#include <pageio.h>

typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;

typedef struct doc {
	int id;
	int count;
} doc_t;

typedef struct rank {
	int rank;
	int id;
    char url[2048];
} rank_t;

static rank_t *make_rank(int rank, int id, char *url) {
    rank_t *rankp = (rank_t*) malloc(sizeof(rank_t));
    
    rankp->rank = rank;
    rankp->id = id;
    strcpy(rankp->url, url);

    return rankp;
}

static void print_rank(void *elementp) {
    rank_t *rankp = (rank_t *) elementp;
    printf("rank: %d: doc: %d : %s\n", rankp->rank, rankp->id, rankp->url);
}

static void free_word(void *elementp){
    word_t *wordp = (word_t*) elementp;

    free(wordp->word);
    qclose(wordp->qdocument);
}

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

static int check_word(char* word, char* pword){
    char* and = "and";
    char* or = "or";
    if (pword == NULL){
        if ((strcmp(word, and) == 0) || (strcmp(word, or) == 0)){
            return -1;
        }
        else{
            return 0;
        }
    }
    if ((strcmp(pword, and) == 0) || (strcmp(pword, or) == 0)){
        if ((strcmp(word, and) == 0) || (strcmp(word, or) == 0)){
            return -2;
        }
        else if (isalpha(word) != 0 || isspace(word)){
            return 0;
        }
        else{
            return -3;
        }
    }

    return 0;
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

/*  Step 1
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

/*  Step 2
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

    int id = 1, rank = -1;
    hashtable_t *htp = indexload("indexnmqs2", "../indices/");

    const char s[2] = " ";
    char *word = strtok(str, s);
    while(word != NULL) {
        word_t *wordp = hsearch(htp, search_word, word, strlen(word));

        if (wordp == NULL) {
            word = strtok(NULL, s);
            rank = 0;
            printf( "%s:%d ", word, rank);
            continue;
        }

        doc_t *docp = qsearch(wordp->qdocument, search_doc, &id);

        if (docp == NULL) {
            word = strtok(NULL, s);
            rank = 0;
            printf( "%s:%d ", word, rank);
            continue;
        }

        if (rank == -1 || docp->count < rank) {
            rank = docp->count;
        }

        printf( "%s:%d ", word, docp->count);
        word = strtok(NULL, s);
    }

    printf("-- %d\n", rank);
    
    free(str);
}
*/

// /*  Step 3
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
    else {
        printf("%s\n", str);
    }

    int last_id = 82;
    hashtable_t *htp = indexload("indexnm", "../indices/");
    queue_t *qp = qopen();

    const char s[2] = " ";
    char *pword = NULL;
    int id;
    for (id = 1; id <= last_id; id++) {
        int rank = -1;
        char sentence[strlen(str) + 1];
        strcpy(sentence, str);
        char *word = strtok(sentence, s);

        while(word != NULL) {
            word_t *wordp = hsearch(htp, search_word, word, strlen(word));

            if (check_word(word, pword) != 0){
            printf("[invalid query]\n");
            exit(EXIT_FAILURE);
            }

            if (wordp == NULL) {
                word = strtok(NULL, s);
                rank = 0;
                continue;
            }

            doc_t *docp = qsearch(wordp->qdocument, search_doc, &id);

            if (docp == NULL) {
                word = strtok(NULL, s);
                rank = 0;
                continue;
            }

            if (rank == -1 || docp->count < rank) {
                rank = docp->count;
            }

            pword = word;
            word = strtok(NULL, s);
        }

        webpage_t *pagep = pageload(id, "../pages-depth3/");
        rank_t *rankp = make_rank(rank, id, webpage_getURL(pagep));

        qput(qp, rankp);
        webpage_delete(pagep);
        free(pagep);
    }

    qapply(qp, print_rank);

    qclose(qp);
    happly(htp, free_word);
    hclose(htp);
    free(str);
}
// */
