#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "pageio.h"
#include "webpage.h"

int32_t pagesave(webpage_t *pagep, int id, char *dirnm) {
    char filename[strlen(dirnm)];
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
    char filename[2];
    sprintf(filename, "%s%d", dirnm, id);

    int result = access(filename, F_OK);
    if (result != 0) {
        printf("File does not exist!!\n");
        return NULL;
    }

    FILE * fp;
    char url[2048], depth[10], HTMLlen[10];
    char *string = (char*) calloc(1, sizeof(char));
    fp = fopen(filename, "r");
    fscanf(fp, "%s\n%s\n%s\n", url, depth, HTMLlen);

    char ch = 0;

    int i = 1;
    while( (ch=fgetc(fp)) != EOF ){
        i++;
        string = (char *) realloc(string, sizeof(char) * i);
        string[i-2] = ch;
        string[i-1] = '\0';
    }

    webpage_t *pagep = webpage_new(url, atoi(depth), string);

    fclose(fp);

    return pagep;
    
}