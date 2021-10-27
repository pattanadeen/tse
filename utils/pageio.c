#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "webpage.h"

int32_t pagesave(webpage_t *pagep, int id, char *dirnm) {
    char filename[strlen(dirnm) + 5];
    int result = access(filename, F_OK);

    if (result == 0) {
        printf("File already exist in the directory\n");
        return -1;
    }

    sprintf(filename, "%s%d", dirnm, id);  

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

    return NULL;
    // webpage_t *pagep = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
}