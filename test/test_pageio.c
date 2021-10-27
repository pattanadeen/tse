#include <stdio.h>
#include <stdlib.h>

#include "pageio.h"
#include "webpage.h"

int main(int argc, char *argv[]) {
    webpage_t *pagep = pageload(1, "../pages-depth3/");
    free(pagep);
}