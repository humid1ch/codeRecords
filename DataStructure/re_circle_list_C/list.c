#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

list* listCreate() {
	list* listHead = malloc(sizeof(listnode));

	if (listHead == NULL) {
		printf("list create faild.\n");
		return NULL;
	}

	listHead->_next = listHead;
	listHead->_prev = listHead;

	return listHead;
}

node* buyNode(int num) {
	node* newNode = (node*)malloc(sizeof(node));

	if (newNode == NULL) {
		printf("buyNode faild.\n");
		return NULL;
	}

	newNode->_data = num;
	newNode->_prev = NULL;
	newNode->_next = NULL;

	return newNode;
}

void printList(const list* list) {
	assert(list);

	const node* cur = list->_next;
	printf("head<-->");
	while (cur != list) {
		printf("%d<-->", cur->_data);
		cur = cur->_next;
	}
	printf("head\n");
}

node* listFind(list* list, int num) {
	assert(list);

	node* cur = list->_next;
	while (cur != list) {
		if (cur->_data == num) {
			return cur;
		}

		cur = cur->_next;
	}

	return NULL;
}

void listPushFront(list* list, int num) {
	assert(list);

	node* newNode = buyNode(num);
	if (newNode == NULL) {
		printf("pushFront faild.\n");
		return;
	}

	node* next =
		list->_next; // 双向循环链表, 头节点的next不可能为空, 最少也是自己

	newNode->_next = next;
	next->_prev = newNode;
	list->_next = newNode;
	newNode->_prev = list;
}

void listPushBack(list* list, int num) {
	assert(list);

	node* newNode = buyNode(num);
	if (newNode == NULL) {
		printf("pushFront faild.\n");
		return;
	}

	node* prev =
		list->_prev; // 双向循环链表, 头节点的next不可能为空, 最少也是自己

	newNode->_next = list;
	list->_prev = newNode;
	prev->_next = newNode;
	newNode->_prev = prev;
}

void listPopFront(list* list) {
	assert(list);

	if (list->_next == list) {
		printf("nothing.\n");
		return;
	}

	node* cur = list->_next;
	list->_next = cur->_next;
	cur->_next->_prev = list;

	free(cur);
}

void listPopBack(list* list) {
	assert(list);

	if (list->_next == list) {
		printf("nothing.\n");
		return;
	}

	node* cur = list->_prev;

	cur->_prev->_next = list;
	list->_prev = cur->_prev;

	free(cur);
}

void listInsert(list* list, int curN, int num) {
	assert(list);

	node* cur = listFind(list, curN);
	if (cur == NULL) {
		printf("nothing.\n");
		return;
	}

	node* newNode = buyNode(num);
	if (newNode == NULL) {
		return;
	}

	node* next = cur->_next;

	newNode->_next = next;
	newNode->_prev = cur;
	next->_prev = newNode;
	cur->_next = newNode;
}

void listErase(list* list, int curN) {
	assert(list);

	node* cur = listFind(list, curN);

	node* prev = cur->_prev;
	node* next = cur->_next;

	prev->_next = next;
	next->_prev = prev;

	free(cur);
}

node* getNode(node* left, node* right) {
    // 保证传入节点不为空, 且两节点之间有节点
    assert(left && right && left != right);
    assert(left->_next != right);
    assert(right->_prev != left);

    node* cur = left->_next;
    left->_next = right;
    right->_prev = left;

    cur->_next = cur;
    cur->_prev = cur;

    return cur;
}
