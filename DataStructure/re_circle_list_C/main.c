#include "list.h"
#include <stdio.h>

int main() {
	list* aList = listCreate();

	for (int i = 0; i < 10; i++) {
		listPushBack(aList, i);
		listPushFront(aList, i * 11);
	}
    printList(aList);

    printf("头删\n");
    listPopFront(aList);
    printList(aList);

    printf("删除22\n");
    listErase(aList, 22);
    printList(aList);

    printf("尾删\n");
    listPopBack(aList);
    printList(aList);

    printf("在11后插入101\n");
    listInsert(aList, 11, 101);
    printList(aList);

	return 0;
}
