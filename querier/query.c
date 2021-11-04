#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static void strip_extra_spaces(char* str) {
    int i, x;

    for(i=x=0; str[i]; ++i) {
        if(!isspace(str[i]) || (i > 0 && !isspace(str[i-1]))) {
            str[x++] = str[i];
        }  
    }

    str[x] = '\n';
    str[x+1] = '\0';
}

static int normalize_sentence(char *str) {
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
            printf("%s", str);
        }
        
        free(str);
    }
}