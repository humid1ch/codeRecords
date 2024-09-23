#include "mylist.h"
#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

kernel_list* listCreate() {
	kernel_lnode* head = (kernel_lnode*)malloc(sizeof(kernel_lnode));
	if (!head) {
		printf("listCreate malloc faild.\n");
		return NULL;
	}

	INIT_LIST_HEAD(&(head->_ptr));

	return head;
}

kernel_lnode* buyNode(int num) {
	kernel_lnode* newNode = (kernel_lnode*)malloc(sizeof(kernel_lnode));
	if (!newNode) {
		printf("buyNode malloc faild.\n");
		return NULL;
	}

	newNode->_data = num;

	INIT_LIST_HEAD(&(newNode->_ptr));

	return newNode;
}

void listPushFront(kernel_list* list, int num) {
	assert(list);

	kernel_lnode* newNode = buyNode(num);
	if (!newNode) {
		printf("buyNode malloc faild.\n");
		return;
	}

	// list_add
	// static inline void list_add(struct list_head *new, struct list_head *head)
	// 默认将 new 放到 head 之后
	list_add(&(newNode->_ptr), &(list->_ptr));
}

void listPushBack(kernel_list* list, int num) {
	assert(list);

	kernel_lnode* newNode = buyNode(num);
	if (!newNode) {
		printf("buyNode malloc faild.\n");
		return;
	}

	// static inline void list_add_tail(struct list_head *new, struct list_head
	// *head) 默认将 new 插入到 head之前
	list_add_tail(&(newNode->_ptr), &(list->_ptr));
}

void listShow(const kernel_list* list) {
	assert(list);

	struct list_head* posNode = NULL;
	kernel_lnode* curNode = NULL;

	printf("head");
	list_for_each(posNode, &(list->_ptr)) {

		// * list_entry – get the struct for this entry
		// * @ptr:    小结构体struct head_list 的结构体指针
		// * @type:    节点大结构体的数据类型
		// * @member:   小结构体在大结构体中的名称.
		curNode = list_entry(posNode, kernel_lnode, _ptr);

		printf("<-->%d", curNode->_data);
	}
	printf("<-->head\n");
}

kernel_lnode* listFind(kernel_list* list, int num) {
	assert(list);

	struct list_head* posNode = NULL;
	kernel_lnode* curNode = NULL;

	list_for_each(posNode, &(list->_ptr)) {
		curNode = list_entry(posNode, kernel_lnode, _ptr);

		if (curNode->_data == num) {
			break;
		}
	}

	return curNode;
}

void listPopBack(kernel_list* list) {
	assert(list);
	assert(list->_ptr.prev != &(list->_ptr));

	struct list_head* posNode = list->_ptr.prev;
	kernel_lnode* curNode = NULL;

	curNode = list_entry(posNode, kernel_lnode, _ptr);
	list_del(posNode);

	free(curNode);
}

void listPopFront(kernel_list* list) {
	assert(list);
	assert(list->_ptr.next != &(list->_ptr));

	struct list_head* posNode = list->_ptr.next;
	kernel_lnode* curNode = NULL;

	curNode = list_entry(posNode, kernel_lnode, _ptr);
	list_del(posNode);

	free(curNode);
}

void listErase(kernel_list* list, int num) {
	assert(list);

	kernel_lnode* curNode = listFind(list, num);
	if (!curNode) {
		printf("Erase faild.\n");
		return;
	}

	list_del(&(curNode->_ptr));

	free(curNode);
}

void listMove(kernel_lnode* node, kernel_list* list) {
	assert(node);
	assert(list);
}
