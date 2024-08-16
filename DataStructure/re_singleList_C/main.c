#include "singleList.h"
#include <stdio.h>
#include <string.h>

int main() {
    printf("%lu\n", strlen("我嘞个都"));

    Node* list = createHead();

    for (int i = 0; i < 10; i++) {
        Node* newNode1 = buyNode(i);
        Node* newNode2 = buyNode(i*11);
        pushFront(list, newNode2);
        pushBack(list, newNode1);
    }

    printList(list);

    return 0;
}
