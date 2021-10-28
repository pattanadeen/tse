#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "pageio.h"
#include "webpage.h"

void print_page(webpage_t *pagep) {
    printf("Url: %s\n", webpage_getURL(pagep));
    printf("Depth: %d\n", webpage_getDepth(pagep));
    printf("HTML Length: %d\n", webpage_getHTMLlen(pagep));
    printf("HTML: %s\n", webpage_getHTML(pagep));
}

int main(int argc, char *argv[]) {

    webpage_t *pagep = pageload(20, "../pages-depth3/");
    print_page(pagep);

    pagesave(pagep, 200, "../pages-depth3/");

    webpage_delete(pagep);
    free(pagep);
}