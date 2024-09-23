#pragma once

typedef struct listnode {
    int _data;
    struct listnode* _prev;
    struct listnode* _next;
}list, listnode, node;

list* listCreate();

node* buyNode(int num);

void printList(const list* list);

node* listFind(list* list, int num);

void listPushFront(list* list, int num);

void listPushBack(list* list, int num);

void listPopFront(list* list);

void listPopBack(list* list);

void listInsert(list* list, int curN, int num);

void listErase(list* list, int curN);

node* getNode(node* left, node* right);
