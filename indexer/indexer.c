#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>

#include <pageio.h>
#include <webpage.h>
#include <lhash.h>
#include <queue.h>
#include <indexio.h>

static int sum;

//  *
//  *  Step 3
//  *
// typedef struct word {
// 	char *word;
// 	int count;
// } word_t;
//  *
//  *  Step 3
//  *


// *
// * Step 4,5,6,7
// *
typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;
// *
// * Step 4,5,6,7
// *

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

bool search_word(void* elementp, const void* keyp){
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

//  *
//  *  Step 3
//  *
// void sumwords(void *elementp){
//     word_t *wordp = (word_t*) elementp;
//     sum = sum + wordp->count;
// }
//  void free_word(void *elementp){
//     word_t *wordp = (word_t*) elementp;

//     free(wordp->word);
// }
//  *
//  *  Step 3
//  *

void qsumwords(void *elementp) {
    doc_t *docp = (doc_t*) elementp;
    sum = sum + docp->count;
}

// *
// * Step 4,5,6,7
// *
void sumwords(void *elementp){
    word_t *wordp = (word_t*) elementp;
    qapply(wordp->qdocument, qsumwords);
}

void free_word(void *elementp){
    word_t *wordp = (word_t*) elementp;

    free(wordp->word);
    qclose(wordp->qdocument);
}
// *
// * Step 4,5,6,7
// *

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

typedef struct arguments {
    lhashtable_t *lhtp;
    int i;
    char pagedir[16];
} arguments_t;

void * routine_index(void * thread_nr) {
    arguments_t *args = (arguments_t *) thread_nr;

    printf("id: %d\n", args->i);

    webpage_t *pagep = pageload(args->i, args->pagedir);
    char *word;
    int pos = 0;

    while ((pos = webpage_getNextWord(pagep, pos, &word)) > 0){
        if(NormalizeWord(word) == 0){
            word_t *words = lhsearch(args->lhtp, search_word, word, strlen(word));
            doc_t *document;
            if(words != NULL){
                if ((document = (doc_t *) qsearch(words->qdocument, search_doc, &(args->i))) != NULL) {
                    document->count = document->count + 1;
                }
                else {
                    qput(words->qdocument, make_doc(args->i, 1));
                }
                free(word);
            }
            else {
                word_t *wordp = malloc(sizeof(word_t));
                wordp->word = word;
                wordp->qdocument = qopen();

                qput(wordp->qdocument, make_doc(args->i, 1));
                lhput(args->lhtp, (void *) wordp, word, strlen(word));
            }
        } else {
            free(word);
        }
    }
    webpage_delete(pagep);
    free(pagep);
    free(word);
    free(args);
    
    return NULL;
}

// void * routine_test(void * thread_nr) {
//     arguments_t *args = (arguments_t *) thread_nr;

//     printf("i: %d\n", args->i);

//     webpage_t *pagep = pageload(args->i, args->pagedir);
//     char *word;
//     int pos = 0;

//     while ((pos = webpage_getNextWord(pagep, pos, &word)) > 0) {
//         printf("%s\n", word);
//         if (NormalizeWord(word) == 0) {
//             word_t *words = lhsearch(args->lhtp, search_word, word, strlen(word));
//             doc_t *document;
//         }
//         else {
//             free(word);
//         }
//     }

//     webpage_delete(pagep);
//     free(pagep);
//     free(word);

//     return NULL;
//     // pthread_exit(NULL);
// }

int main(int argc, char *argv[]) {

    /*
     *  Step 3
     *
    hashtable_t *htp = hopen(10);
    webpage_t *pagep = pageload(1, "../pages-depth3/");
    char *word;
    int pos = 0;

    while ((pos = webpage_getNextWord(pagep, pos, &word)) > 0){
        if(NormalizeWord(word) == 0){
            // printf("%s\n", word);
            word_t *words = hsearch(htp, search_word, word, strlen(word));

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

    happly(htp, free_word);
    hclose(htp);

    exit(EXIT_SUCCESS);
     *
     *  Step 3
     */

    /*
     *  Step 4,5
     *
    if (argv[1] == NULL){
        printf("Please insert id file\n");
        exit(EXIT_FAILURE);
    }
    int id = atoi(argv[1]);
    hashtable_t *htp = hopen(10);
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
    }    
    
    // sum = 0;
    // happly(htp, sumwords);
    // printf("%d", sum);

    indexsave(htp, "indexnm", "../indices/");

    happly(htp, free_word);
    hclose(htp);

    exit(EXIT_SUCCESS);
    *
    *  Step 4,5
    */

    /*
     *  Step 7
     *
   if(argc!=3 || argv[1] == NULL || argv[2] == NULL){
        printf("usage: indexer <pagedir> <indexnm>\n");
        exit(EXIT_FAILURE);
    }

    char *pagedir_temp = argv[1];
    char *indexnm = argv[2];
    char pagedir[16];
    sprintf(pagedir, "%s%s%s", "../", pagedir_temp, "/");
    printf("%s\n", pagedir);
    printf("%s\n", indexnm);
    
    hashtable_t *htp = hopen(10);

    int count = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(pagedir);

    if (d) {
        printf("exist");
    } else if (ENOENT == errno) {
        printf("not exist");
        return -2;
    } else {
        return -1;
        printf("failed exist");
    }

    if(d){
        while ((dir = readdir(d)) != NULL){
            count++;
        }
    }
    count = count - 2;
    int i = 1;

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

    exit(EXIT_SUCCESS);
    *
    *  Step 7
    */

    // /*  Module 7
    if(argc!=4 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL){
        printf("usage: indexer <pagedir> <indexnm> <num thread>\n");
        exit(EXIT_FAILURE);
    }

    char *pagedir_temp = argv[1];
    char *indexnm = argv[2];
    int num_threads = atoi(argv[3]);
    char pagedir[16];
    sprintf(pagedir, "%s%s%s", "../", pagedir_temp, "/");
    printf("%s\n", pagedir);
    printf("%s\n", indexnm);
    
    hashtable_t *lhtp = lhopen(10);

    int count = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(pagedir);

    if (d) {
        printf("exist\n");
    } else if (ENOENT == errno) {
        printf("not exist\n");
        return -2;
    } else {
        return -1;
        printf("failed exist\n");
    }

    if(d){
        while ((dir = readdir(d)) != NULL){
            count++;
        }
    }
    count = count - 2;
    int i = 1;

    if (d) {
        while (i != count+1){
            int max_thread, id = i;

            if ((float) count / (float) (i + num_threads - 1) < 1.0) {
                max_thread = count % num_threads;
            }
            else {
                max_thread = num_threads;
            }

            pthread_t thread[max_thread];
            arguments_t *temp_arg[max_thread];

            for (int current_t = 0; current_t < max_thread; current_t++) {
                arguments_t *arg = malloc(sizeof(arguments_t));
                arg->i = id;
                arg->lhtp = lhtp;
                strcpy(arg->pagedir, pagedir);

                temp_arg[current_t] = arg;

                if(pthread_create(&thread[current_t], NULL, routine_index, temp_arg[current_t])!=0) {
                    exit(EXIT_FAILURE);
                }

                id++;
            }

            for (int current_t = 0; current_t < max_thread; current_t++) {
                if (pthread_join(thread[current_t], NULL)!=0) {
                    exit(EXIT_FAILURE);
                }
            }

            i += max_thread;
        }

        closedir(d); 
    }   

    lindexsave(lhtp, indexnm, "../indices/");

    lhapply(lhtp, free_word);
    lhclose(lhtp);

    exit(EXIT_SUCCESS);
    // */  
}
