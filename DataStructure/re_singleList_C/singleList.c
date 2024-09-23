#include "singleList.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Node* createHead() {
	Node* head = (Node*)malloc(sizeof(Node));
	if (head == NULL) {
		printf("createHead faild.\n");
		return NULL;
	}

    head->_data = 0;
    head->_next = NULL;

	return head;
}

Node* buyNode(int num) {
    Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("buyNode faild.\n");
		return NULL;
	}

    newNode->_next = NULL;
    newNode->_data = num;

    return newNode;
}

void pushBack(Node* list, Node* node) {
    assert(list);
    assert(node);

    Node* cur = list;
    while (cur->_next != NULL) {
        cur = cur->_next;
    }

    cur->_next = node;
}

void pushFront(Node *list, Node *node) {
    assert(list);
    assert(node);

    Node* next = list->_next;
    list->_next = node;
    node->_next = next;
}

void printList(Node* list) {
    assert(list);

    Node* cur = list;
    while (cur != NULL) {
        if (cur == list) {
            printf("head->");
        }
        else {
            printf("%d->", cur->_data);
        }

        cur = cur->_next;
    }

    printf("NULL\n");
}

Node* findNode(Node* list, int num) {
    assert(list);

    Node* cur = list;
    while (cur->_next != NULL) {
        if (cur->_data == num) {
            return cur;
        }

        cur = cur->_next;
    }

    return NULL;
}
