#include "list.h"
#include "orderList.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    list* aList = createList();

    int num = 0;
    printf("请输入随机链表长度(10~20):> ");
    scanf("%d", &num);
    
    srand(time(0));
    for (int i = 0; i < num; i++) {
        listPushBack(aList, rand() % 100);
    }
    printList(aList);

    printf("此链表的总结节点值为: %d\n", midNode(aList));

//     for (int i = 0; i < 20; i++) {
//         int num = 0;
//         printf("请输入一个整数:> ");
//         scanf("%d", &num);
// 
//         orderInsert(aList, num);
//         printList(aList);
//     }
//     printList(aList);

    //     for (int i = 0; i < 10; i++) {
    //         listPushBack(aList, i);
    //         listPushFront(aList, i * 11);
    //     }
    //     printList(aList);
    //     printf("\n");
    //
    //     printf("尾删\n");
    //     listPopBack(aList);
    //     printList(aList);
    //     printf("\n");
    //
    //     printf("删5\n");
    //     listErase(aList, 5);
    //     printList(aList);
    //     printf("\n");
    //
    //     printf("头删\n");
    //     listPopFront(aList);
    //     printList(aList);
    //     printf("\n");
    //
    //     printf("6后插100\n");
    //     listInsertAfter(aList, 6, 100);
    //     printList(aList);
    //     printf("\n");
    //
    //     printf("6前插200\n");
    //     listInsertBefore(aList, 6, 200);
    //     printList(aList);
    //     printf("\n");
    //
    //     printf("链表一共%d个有效节点\n", listSize(aList));
    //     printList(aList);
    //     printf("\n");

    return 0;
}
