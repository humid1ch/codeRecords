#pragma once

#include "list.h"


// 航班信息数据节点（其成员可适当增删）
typedef struct flight {
	char _number[10];	  // 航班号
	char _staddress[10];  // 起点站
	char _arraddress[10]; // 终点站
	char _date[10];		  // 班期
	char _type;			  // 机型
	int _stime;			  // 起飞时间
	int _atime;			  // 到达时间
	float _price;		  // 票价
} datatype;

typedef struct FlyListNode {
    datatype _data;
    struct list_head _ptr;
} FlyListNode, FlyList;

FlyList* createList();

FlyListNode* buyNode(const datatype* fly);

void listPushFront(FlyList* list, datatype* fly);

void listAddTail(FlyList* list, datatype* fly);

void listMove(FlyList* list, FlyListNode* flyNode);

void listShow(const FlyList* list);

// 根据日期查, 函数内新建链表
FlyList* listFindByDate(FlyList* list, char* date);

// 根据起点站查
FlyList* listFindBySaddr(FlyList* list, char* saddr);

// 根据终点站查
FlyList* listFindByAaddr(FlyList* list, char* saddr);

// 根据机型查
FlyList* listFindByType(FlyList* list, char type);

// 建议不在原链表排序
FlyList* listSortByStime(FlyList* list);

FlyList* listSortBySaddr(FlyList* list);

FlyList* listSortByPrice(FlyList* list);
