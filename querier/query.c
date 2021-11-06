#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

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

int sum;

static void sum_rank(void *elementp) {
    rank_t *rankp = (rank_t *) elementp;
    sum += rankp->rank;
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
        return -1;
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
    }

    return 0;
}

static int sum_array(int *arr, int size) {
    int sum = 0, i;

    for(i = 0; i < size; i++) {
        sum += arr[i];
    }          

    return sum;
}

static void trap_exit(hashtable_t *htp, queue_t *qp, char *str) {
    qclose(qp);
    happly(htp, free_word);
    hclose(htp);
    free(str);
}

//
// indexer
//

doc_t *make_doc(int docID, int count) {
    doc_t *docp = (doc_t*) malloc(sizeof(doc_t));
    docp->id = docID;
    docp->count = count;
    return docp;
}

void qsumwords(void *elementp) {
    doc_t *docp = (doc_t*) elementp;
    sum = sum + docp->count;
}

void sumwords(void *elementp){
    word_t *wordp = (word_t*) elementp;
    qapply(wordp->qdocument, qsumwords);
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

/*  Step 3
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
*/

// /*  Step 4
// int main(int argc, char *argv[]) {
//     char *str = (char *) malloc(sizeof(char));
//     char cha;
//     int i = 1;

//     while (1) {
//         if (scanf("%c", &cha) == -1) {
//             printf("\n[invalid query]\n");

//             free(str);
//             exit(EXIT_SUCCESS);
//         }

//         i++;
//         str = (char *) realloc(str, sizeof(char) * i);
//         str[i-2] = cha;
//         str[i-1] = '\0';

//         if (cha == '\n') {
//             break;
//         }            
//     }

//     if (normalize_sentence(str) == -1) {
//         printf("[invalid query]\n");

//         free(str);
//         exit(EXIT_FAILURE);
//     }
//     else {
//         printf("%s\n", str);
//     }

//     int last_id = 82;
//     hashtable_t *htp = indexload("indexnm", "../indices/");
//     queue_t *qp = qopen();

//     const char s[2] = " ";
//     char *pword = NULL;
//     int id;
//     for (id = 1; id <= last_id; id++) {
//         int rank = -1;
//         char sentence[strlen(str) + 1];
//         strcpy(sentence, str);
//         char *word = strtok(sentence, s);
//         int *ranks = (int *) malloc(sizeof(int));
//         int i = 1;

//         while(word != NULL) {
//             if (check_word(word, pword) != 0){
//                 printf("[invalid query]\n");
                
//                 free(ranks);
//                 trap_exit(htp, qp, str);
//                 exit(EXIT_FAILURE);
//             }

//             if (strcmp(word, "and") == 0) {
//                 pword = word;
//                 word = strtok(NULL, s);

//                 if (word == NULL) {
//                     printf("[invalid query]\n");

//                     free(ranks);
//                     trap_exit(htp, qp, str);
//                     exit(EXIT_FAILURE);
//                 }
//                 else {
//                     continue;
//                 }
//             }
//             else if (strcmp(word, "or") == 0) {
//                 ranks = (int *) realloc(ranks, sizeof(int) * i);
//                 ranks[i - 1] = rank;
//                 rank = -1;
//                 i++;

//                 pword = word;
//                 word = strtok(NULL, s);

//                 if (word == NULL) {
//                     printf("[invalid query]\n");

//                     trap_exit(htp, qp, str);
//                     exit(EXIT_FAILURE);
//                 }
//                 else {
//                     continue;
//                 }
//             }

//             word_t *wordp = hsearch(htp, search_word, word, strlen(word));

//             if (wordp == NULL) {
//                 word = strtok(NULL, s);
//                 rank = 0;
//                 continue;
//             }

//             doc_t *docp = qsearch(wordp->qdocument, search_doc, &id);

//             if (docp == NULL) {
//                 word = strtok(NULL, s);
//                 rank = 0;
//                 continue;
//             }

//             if (rank == -1 || docp->count < rank) {
//                 rank = docp->count;
//             }

//             pword = word;
//             word = strtok(NULL, s);
//         }

//         rank += sum_array(ranks, i - 1);

//         webpage_t *pagep = pageload(id, "../pages-depth3/");
//         rank_t *rankp = make_rank(rank, id, webpage_getURL(pagep));
//         qput(qp, rankp);

//         webpage_delete(pagep);
//         free(pagep);
//         free(ranks);
//     }

//     sum = 0;
//     qapply(qp, sum_rank);
//     if (sum == 0) {
//         printf("[invalid query]\n");
//     }
//     else {
//         qapply(qp, print_rank);
//     }

//     trap_exit(htp, qp, str);
//     exit(EXIT_SUCCESS);
// }
// */

// step 5
int main(int argc, char *argv[]) {
    // if(argv[3] == "q"){

    //     exit(EXIT_SUCCESS);
    // }
    if(argc!=3 || argv[1] == NULL || argv[2] == NULL){
        printf("usage: indexer <pagedir> <indexnm> [-q]\n");
        exit(EXIT_FAILURE);
    }

    char *pagedir_temp = argv[1];
    char *indexnm = argv[2];
    char pagedir[16];
    sprintf(pagedir, "%s%s%s", "../", pagedir_temp, "/");
    // printf("%s\n", pagedir);
    // printf("%s\n", indexnm);
    
    hashtable_t *htp = hopen(10);

    int count = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(pagedir);

    if (d) {
        printf("directory exist\n");
    } else if (ENOENT == errno) {
        printf("directory does not exist\n");
        closedir(d);
        happly(htp, free_word);
        hclose(htp);
        exit(EXIT_FAILURE);
    } else {
        printf("Directory is not accessible\n");
        closedir(d);
        happly(htp, free_word);
        hclose(htp);
        exit(EXIT_FAILURE);
    }

    if (d){
        while ((dir = readdir(d)) != NULL){
            count++;
        }
    }
    count = count - 2;
    int i = 1;

    if(count == 0){
        printf("Directory is empty or has not been crawled\n");
        closedir(d);
        happly(htp, free_word);
        hclose(htp);
        exit(EXIT_FAILURE);
    }
    

    if (d){
        while (i != count+1){
            // printf("%s\n", dir->d_name);
            printf("%d\n", i);

            webpage_t *pagep = pageload(i, pagedir);
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
                        free(word);
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
            i++;
        }
            closedir(d); 
    }   


    indexsave(htp, indexnm, "../indices/");

    happly(htp, free_word);
    hclose(htp);

    // exit(EXIT_SUCCESS);
    //
    char *str = (char *) malloc(sizeof(char));
    char cha;
    i = 1;

    while (1) {
        if (scanf("%c", &cha) == -1) {
            printf("\n[invalid query]\n");

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

    if(!access(indexnm, R_OK )){
        printf("The File %s cannot be read\n",indexnm);
        closedir(d);
        happly(htp, free_word);
        hclose(htp);
        exit(EXIT_FAILURE);
    }

    int last_id = 82;
    hashtable_t *htpq = indexload(indexnm, "../indices/");
    queue_t *qp = qopen();

    const char s[2] = " ";
    char *pword = NULL;
    int id;
    for (id = 1; id <= last_id; id++) {
        int rank = -1;
        char sentence[strlen(str) + 1];
        strcpy(sentence, str);
        char *word = strtok(sentence, s);
        int *ranks = (int *) malloc(sizeof(int));
        int i = 1;

        while(word != NULL) {
            if (check_word(word, pword) != 0){
                printf("[invalid query]\n");
                
                free(ranks);
                trap_exit(htpq, qp, str);
                exit(EXIT_FAILURE);
            }

            if (strcmp(word, "and") == 0) {
                pword = word;
                word = strtok(NULL, s);

                if (word == NULL) {
                    printf("[invalid query]\n");

                    free(ranks);
                    trap_exit(htpq, qp, str);
                    exit(EXIT_FAILURE);
                }
                else {
                    continue;
                }
            }
            else if (strcmp(word, "or") == 0) {
                ranks = (int *) realloc(ranks, sizeof(int) * i);
                ranks[i - 1] = rank;
                rank = -1;
                i++;

                pword = word;
                word = strtok(NULL, s);

                if (word == NULL) {
                    printf("[invalid query]\n");

                    trap_exit(htpq, qp, str);
                    exit(EXIT_FAILURE);
                }
                else {
                    continue;
                }
            }

            word_t *wordp = hsearch(htpq, search_word, word, strlen(word));

            if (wordp == NULL) {
                pword = word;
                word = strtok(NULL, s);
                rank = 0;
                continue;
            }

            doc_t *docp = qsearch(wordp->qdocument, search_doc, &id);

            if (docp == NULL) {
                pword = word;
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

        rank += sum_array(ranks, i - 1);

        webpage_t *pagep = pageload(id, "../pages-depth3/");
        rank_t *rankp = make_rank(rank, id, webpage_getURL(pagep));
        qput(qp, rankp);

        webpage_delete(pagep);
        free(pagep);
        free(ranks);
    }

    sum = 0;
    qapply(qp, sum_rank);
    if (sum == 0) {
        printf("[invalid query]\n");
    }
    else {
        qapply(qp, print_rank);
    }

    trap_exit(htpq, qp, str);
    exit(EXIT_SUCCESS);
}