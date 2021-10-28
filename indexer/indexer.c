#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "pageio.h"
#include "webpage.h"

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

    while ((pos = webpage_getNextWord(pagep, pos, &word)) > 0){
        if(NormalizeWord(word) == 0){
            printf("%s\n", word);
        }
    }

    webpage_delete(pagep);
    free(pagep);
    free(word);

    return 0;
}