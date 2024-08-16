#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

list* createList() {
    list* head = (list*)malloc(sizeof(listnode));
    if (head == NULL) {
        printf("list create faild.\n");
        return NULL;
    }

    head->_data = 0;
    head->_next = NULL;
    head->_prev = NULL;

    return head;
}

listnode* buyListnode(int num) {
    listnode* newNode = (listnode*)malloc(sizeof(listnode));
    if (newNode == NULL) {
        printf("buyListnode faild.\n");
        return NULL;
    }

    newNode->_prev = NULL;
    newNode->_next = NULL;
    newNode->_data = num;

    return newNode;
}

void listPushBack(list* list, int num) {
    assert(list);

    listnode* newNode = buyListnode(num);
    listnode* cur = list;
    while (cur->_next != NULL) {
        cur = cur->_next;
    }

    cur->_next = newNode;
    newNode->_prev = cur;
    newNode->_next = NULL;
}

void listPushFront(list* list, int num) {
    assert(list);

    listnode* newNode = buyListnode(num);

    newNode->_next = list->_next;
    if (list->_next != NULL) {
        list->_next->_prev = newNode;
    }
    newNode->_prev = list;
    list->_next = newNode;
}

listnode* listFind(list* list, int num) {
    assert(list);

    listnode* cur = list->_next;
    while (cur != NULL) {
        if (cur->_data == num) {
            return cur;
        }
        cur = cur->_next;
    }

    return NULL;
}

void listInsertAfter(list* list, int curNum, int num) {
    assert(list);

    listnode* newNode = buyListnode(num);
    listnode* cur = listFind(list, curNum);
    if (cur == NULL) {
        printf("Insert faild, reason: nothing.\n");
        return;
    }

    if (cur->_next == NULL) {
        cur->_next = newNode;
        newNode->_prev = cur;
        newNode->_next = NULL;
        return;
    }

    newNode->_next = cur->_next;
    newNode->_next->_prev = newNode;
    cur->_next = newNode;
    newNode->_prev = cur;
}

void listInsertBefore(list* list, int curNum, int num) {
    assert(list);

    listnode* newNode = buyListnode(num);
    listnode* cur = listFind(list, curNum);
    if (cur == NULL) {
        printf("Insert faild, reason: nothing.\n");
        return;
    }

    listnode* prev = cur->_prev;

    newNode->_next = cur;
    newNode->_prev = prev;
    cur->_prev = newNode;
    prev->_next = newNode;
}

void listErase(list* list, int num) {
    assert(list);

    listnode* cur = listFind(list, num);
    if (cur == NULL) {
        printf("Erase faild, reason: nothing.\n");
        return;
    }

    // 尾
    if (cur->_next == NULL) {
        cur->_prev->_next = NULL;
        free(cur);

        return;
    }

    // 非尾
    listnode* prev = cur->_prev;
    listnode* next = cur->_next;

    prev->_next = cur->_next;
    next->_prev = cur->_prev;

    free(cur);
}

void listPopBack(list* list) {
    assert(list);

    listnode* cur = list;
    while (cur->_next != NULL) {
        cur = cur->_next;
    }

    if (cur == list) {
        printf("popBack faild, reason: nothing.\n");
        return;
    }

    cur->_prev->_next = NULL;
    free(cur);
}

void listPopFront(list* list) {
    assert(list);

    listnode* cur = list->_next;
    if (cur == NULL) {
        printf("popBack faild, reason: nothing.\n");
        return;
    }

    list->_next = cur->_next;
    if (cur->_next != NULL) 
       cur->_next->_prev = list;

    free(cur);
}

int listSize(const list* list) {
    assert(list);

    int cnt = 0;

    const listnode* cur = list->_next;
    while (cur) {
        cnt++;
        cur = cur->_next;
    }

    return cnt;
}

void printList(const list* list) {
    assert(list);

    const listnode* cur = list->_next;
    printf("head<->");
    while (cur != NULL) {
        printf("%d<->", cur->_data);
        cur = cur->_next;
    }
    printf("NULL\n");
}
