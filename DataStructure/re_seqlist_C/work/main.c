#include "seqlist.h"
#include <stdio.h>

int main() {
    BookSL books;
    booksInit(&books, 4);

    for (int i = 0; i < books._capacity; i++) {
        Book book;
        booksAddBook(&books, &book);
    }
    booksShow(&books);

    booksEraseByName(&books);
    printf("价格升序排列\n");
    ascendSortByPrice(&books);
    booksShow(&books);

    booksModifyByName(&books);
    printf("价格降序排列\n");
    descendSortByPrice(&books);
    booksShow(&books);

    return 0;
}
