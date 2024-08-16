#include "list.h"
#include <stdio.h>

int main() {
	list* aList = createList();

	for (int i = 0; i < 10; i++) {
		listPushBack(aList, i);
		listPushFront(aList, i * 11);
	}
	printList(aList);
    printf("\n");

	printf("尾删\n");
    listPopBack(aList);
	printList(aList);
    printf("\n");

	printf("删5\n");
    listErase(aList, 5);
	printList(aList);
    printf("\n");
	
    printf("头删\n");
    listPopFront(aList);
	printList(aList);
    printf("\n");

	printf("6后插100\n");
	listInsertAfter(aList, 6, 100);
	printList(aList);
    printf("\n");

	printf("6前插200\n");
	listInsertBefore(aList, 6, 200);
	printList(aList);
    printf("\n");

	printf("链表一共%d个有效节点\n", listSize(aList));
	printList(aList);
    printf("\n");

	return 0;
}
