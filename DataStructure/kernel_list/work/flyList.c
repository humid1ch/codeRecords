#include "flyList.h"
#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// flys 中存的是 FlyListNode 节点地址
void ascendSortByStime(FlyListNode** flys, int size) {
	int gap = size;

	while (gap > 1) {
		gap = gap / 3 + 1;
		for (int i = 0; i < size - gap; i++) {
			int end = i;
			FlyListNode* cur = flys[end + gap];
			int curStime = cur->_data._stime;
			while (end >= 0) {
				if (curStime < flys[end]->_data._stime) {
					flys[end + gap] = flys[end];

					end -= gap;
				}
				else {
					break;
				}
			}
			flys[end + gap] = cur;
		}
	}
}

void ascendSortBySaddr(FlyListNode** flys, int size) {
	int gap = size;

	while (gap > 1) {
		gap = gap / 3 + 1;
		for (int i = 0; i < size - gap; i++) {
			int end = i;
			FlyListNode* cur = flys[end + gap];
			char* curSaddr = cur->_data._staddress;
			while (end >= 0) {
				if (strcmp(curSaddr, flys[end]->_data._staddress) < 0) {
					flys[end + gap] = flys[end];

					end -= gap;
				}
				else {
					break;
				}
			}
			flys[end + gap] = cur;
		}
	}
}

void ascendSortByPrice(FlyListNode** flys, int size) {
	int gap = size;

	while (gap > 1) {
		gap = gap / 3 + 1;
		for (int i = 0; i < size - gap; i++) {
			int end = i;
			FlyListNode* cur = flys[end + gap];
			int curPrice = cur->_data._price;
			while (end >= 0) {
				if (curPrice < flys[end]->_data._price) {
					flys[end + gap] = flys[end];

					end -= gap;
				}
				else {
					break;
				}
			}
			flys[end + gap] = cur;
		}
	}
}

FlyList* createList() {
	FlyList* head = (FlyListNode*)malloc(sizeof(FlyListNode));
	if (!head) {
		printf("list create: malloc faild.\n");
		return NULL;
	}

	INIT_LIST_HEAD(&(head->_ptr));

	return head;
}

FlyListNode* buyNode(const datatype* fly) {
	FlyListNode* newNode = (FlyListNode*)malloc(sizeof(FlyListNode));
	if (!newNode) {
		printf("buy node: malloc faild.\n");
		return NULL;
	}

	strcpy(newNode->_data._number, fly->_number);
	strcpy(newNode->_data._staddress, fly->_staddress);
	strcpy(newNode->_data._arraddress, fly->_arraddress);
	strcpy(newNode->_data._date, fly->_date);
	newNode->_data._type = fly->_type;
	newNode->_data._stime = fly->_stime;
	newNode->_data._atime = fly->_atime;
	newNode->_data._price = fly->_price;

	INIT_LIST_HEAD(&(newNode->_ptr));

	return newNode;
}

// 头插
void listPushFront(FlyList* list, datatype* fly) {
	assert(list);

	FlyListNode* newNode = buyNode(fly);
	if (!newNode) {
		printf("push front faild.\n");
		return;
	}

	list_add(&(newNode->_ptr), &(list->_ptr));
}

void listAddTail(FlyList* list, datatype* fly) {
	assert(list);

	FlyListNode* newNode = buyNode(fly);
	if (!newNode) {
		printf("push front faild.\n");
		return;
	}

	list_add_tail(&(newNode->_ptr), &(list->_ptr));
}

void listMove(FlyList* list, FlyListNode* flyNode) {
	assert(list);
	assert(flyNode);

	list_move_tail(&(flyNode->_ptr), &(list->_ptr));
}

void listShow(const FlyList* list) {
	assert(list);

	struct list_head* posNode = NULL;
	FlyListNode* curNode = NULL;

	printf("航班号\t起点-->终点\t班期\t机型\t起飞时间\t到达时间\t票价\n");

	// list_for_each 本质上是一个 不包含循环体的for循环
	list_for_each(posNode, &(list->_ptr)) {
		// list_entry 本质上是一个宏定义: list_entry(pos, type, member)
		// pos 是目标list_head变量, type是以list_head为成员变量的结构体类型,
		// member是type内list_head成员变量的变量名
		curNode = list_entry(posNode, FlyListNode, _ptr);
		printf("%s\t%s--->%s\t%s\t%c\t%d\t\t%d\t\t%.2f\n", curNode->_data._number,
			   curNode->_data._staddress, curNode->_data._arraddress,
			   curNode->_data._date, curNode->_data._type, curNode->_data._stime,
			   curNode->_data._atime, curNode->_data._price);
	}
}

// 根据日期查, 函数内新建链表
FlyList* listFindByDate(FlyList* list, char* date) {
	assert(list);

	FlyList* dateList = createList(); // 创建新链表

	FlyListNode* curNode = NULL;

	list_for_each_entry(curNode, &(list->_ptr), _ptr) {
		if (!strcmp(curNode->_data._date, date)) {
			listAddTail(dateList, &(curNode->_data));
		}
	}

	return dateList;
}

// 根据起点站查
FlyList* listFindBySaddr(FlyList* list, char* saddr) {
	assert(list);

	FlyList* saddrList = createList(); // 创建新链表

	FlyListNode* curNode = NULL;

	list_for_each_entry(curNode, &(list->_ptr), _ptr) {
		if (!strcmp(curNode->_data._staddress, saddr)) {
			listAddTail(saddrList, &(curNode->_data));
		}
	}

	return saddrList;
}

// 根据终点站查
FlyList* listFindByAaddr(FlyList* list, char* aaddr) {
	assert(list);

	FlyList* aaddrList = createList(); // 创建新链表

	FlyListNode* curNode = NULL;

	list_for_each_entry(curNode, &(list->_ptr), _ptr) {
		if (!strcmp(curNode->_data._arraddress, aaddr)) {
			listAddTail(aaddrList, &(curNode->_data));
		}
	}

	return aaddrList;
}

// 根据机型查
FlyList* listFindByType(FlyList* list, char type) {
	assert(list);

	FlyList* typeList = createList(); // 创建新链表

	FlyListNode* curNode = NULL;

	list_for_each_entry(curNode, &(list->_ptr), _ptr) {
		if (curNode->_data._type == type) {
			listAddTail(typeList, &(curNode->_data));
		}
	}

	return typeList;
}

// 不建议直接在原链表排序
FlyList* listSortByStime(FlyList* list) {
	assert(list);

	FlyList* newList = createList();

	struct list_head* posNode = NULL;
	FlyListNode* curNode = NULL;

	int cnt = 0;
	list_for_each(posNode, &(list->_ptr)) {
		cnt++;
	}

	FlyListNode* flys[cnt];
	memset(flys, 0, sizeof(FlyListNode*) * cnt);

	int i = 0;
	list_for_each_entry(curNode, &(list->_ptr), _ptr) {
		flys[i++] = curNode;
	}

	// 对数组排序
	ascendSortByStime(flys, cnt);

	for (i = 0; i < cnt; i++) {
		listMove(newList, flys[i]);
	}

	// 原链表元素全部被转移, 链表头无用
	list->_ptr.next = NULL;
	list->_ptr.prev = NULL;
	free(list);

	return newList;
}

FlyList* listSortBySaddr(FlyList* list) {
	assert(list);

	FlyList* newList = createList();

	struct list_head* posNode = NULL;
	FlyListNode* curNode = NULL;

	int cnt = 0;
	list_for_each(posNode, &(list->_ptr)) {
		cnt++;
	}

	FlyListNode* flys[cnt];
	memset(flys, 0, sizeof(FlyListNode*) * cnt);

	int i = 0;
	list_for_each_entry(curNode, &(list->_ptr), _ptr) {
		flys[i++] = curNode;
	}

	// 对数组排序
	ascendSortBySaddr(flys, cnt);

	for (i = 0; i < cnt; i++) {
		listMove(newList, flys[i]);
	}

	// 原链表元素全部被转移, 链表头无用
	list->_ptr.next = NULL;
	list->_ptr.prev = NULL;
	free(list);

	return newList;
}

FlyList* listSortByPrice(FlyList* list) {
	assert(list);

	FlyList* newList = createList();

	struct list_head* posNode = NULL;
	FlyListNode* curNode = NULL;

	int cnt = 0;
	list_for_each(posNode, &(list->_ptr)) {
		cnt++;
	}

	FlyListNode* flys[cnt];
	memset(flys, 0, sizeof(FlyListNode*) * cnt);

	int i = 0;
	list_for_each_entry(curNode, &(list->_ptr), _ptr) {
		flys[i++] = curNode;
	}

	// 对数组排序
	ascendSortByPrice(flys, cnt);

	for (i = 0; i < cnt; i++) {
		listMove(newList, flys[i]);
	}

	// 原链表元素全部被转移, 链表头无用
	list->_ptr.next = NULL;
	list->_ptr.prev = NULL;
	free(list);

	return newList;
}

// 用于释放链表
void freeFlyList(FlyList* list) {
	assert(list);
	// assert(list_empty(&(list->_ptr)));

	struct list_head* posNode = NULL;
	FlyListNode* curNode = NULL;

	// 释放各节点
	list_for_each(posNode, &(list->_ptr)) {
		curNode = list_entry(posNode, FlyListNode, _ptr);

		list_del(posNode);
		free(curNode);
	}

	// 释放头节点
	free(list);
}
