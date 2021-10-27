#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "pageio.h"
#include "webpage.h"

int32_t pagesave(webpage_t *pagep, int id, char *dirnm) {
    char filename[strlen(dirnm) + 5];
    sprintf(filename, "%s%d", dirnm, id);
    int result = access(filename, F_OK);

    if (result == 0) {
        printf("File already exist in the directory!!\n");
        return -1;
    }

    FILE * fp;
    fp = fopen(filename, "w");

    fprintf(fp, "%s\n", webpage_getURL(pagep));
    fprintf(fp, "%d\n", webpage_getDepth(pagep));
    fprintf(fp, "%d\n", webpage_getHTMLlen(pagep));
    fprintf(fp, "%s", webpage_getHTML(pagep));
    
    fclose(fp);

    return 0;
}

webpage_t *pageload(int id, char *dirnm) {
    char filename[strlen(dirnm) + 1];
    sprintf(filename, "%s%d", dirnm, id);
    int result = access(filename, F_OK);

    if (result != 0) {
        printf("File does not exist!!\n");
        return NULL;
    }

    char str1[100], str2[100], str3[100], str4[1000];
    FILE * fp;
    fp = fopen(filename, "r");

    fscanf(fp, "%s\n%s\n%s", str1, str2, str3, str4);
    printf("Read String1 |%s|\n", str1);
    printf("Read String1 |%s|\n", str2);
    printf("Read String1 |%s|\n", str3);
    printf("Read String1 |%s|\n", str4);

    fclose(fp);

    return NULL;
    // webpage_t *pagep = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
}