#pragma once

typedef struct listnode {
	int _data;
	struct listnode* _prev;
	struct listnode* _next;
} listnode, list;

list* createList();

listnode* buyListnode(int num);

void listPushBack(list* list, int num);

void listPushFront(list* list, int num);

listnode* listFind(list* list, int num);

listnode* listFindPos(list* list, int num);

void printList(const list* list);

void listInsertAfter(list* list, int curNum, int num);

void listInsertBefore(list* list, int curNum, int num);

void listErase(list* list, int num);

void listPopBack(list* list);

void listPopFront(list* list); 

int listSize(const list* list);

void orderInsert(list* list, int num);

int midNode(const list* list);
