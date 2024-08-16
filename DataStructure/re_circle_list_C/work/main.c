#include "JosephRing.h"
#include "list.h"
#include <stdio.h>

int main() {
    list* aList = listCreate();

    for (int i = 0; i < 20; i++) {
        listPushBack(aList, i);
    }
    printList(aList);
    printf("\n");

    josephRing(aList);

    return 0;
}
