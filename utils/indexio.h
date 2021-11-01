#pragma once
#include <inttypes.h>

#include "hash.h"

int32_t indexsave(hashtable_t *htp, char *indexnm, char *dirnm);
hashtable_t *indexload(char *indexnm, char*dirnm);
