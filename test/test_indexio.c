

typedef struct word {
	char *word;
	queue_t *qdocument;
} word_t;

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

int main(int argc, char *argv[]) {

    pagesave(pagep, 200, "../pages-depth3/");

    webpage_delete(pagep);
    free(pagep);
}