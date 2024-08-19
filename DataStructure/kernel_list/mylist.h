#pragma once

#include "list.h"

typedef struct kernel_lnode {
	int _data;
	struct list_head _ptr;
} kernel_list, kernel_lnode;

kernel_list* listCreate();

kernel_lnode* buyNode(int num);

void listPushFront(kernel_list* list, int num);

void listPushBack(kernel_list* list, int num);

void listShow(const kernel_list* list);

kernel_lnode* listFind(kernel_list* list, int num);

void listPopBack(kernel_list* list);

void listPopFront(kernel_list* list);

void listErase(kernel_list* list, int num);

void listMove(kernel_lnode* node, kernel_list* list);
